#include "ImGuiCreator.h"

#include "imgui_impl_dx11.h"
#include "Object.h"
#include "SceneEditor.h"
#include "SceneDataManager.h"


bool ImGuiCreator::PutPointerField(std::string& serializedData, const std::string& serializedVarName, const std::string& label, const std::string& instanceID)
{
	bool outHasChanged = false;

	ImGui::Text(label.c_str());
	if (ImGui::Button(instanceID.c_str()))
	{
		ImGui::OpenPopup("select_instanceID_popup");
	}
	if (ImGui::BeginPopup("select_instanceID_popup"))
	{
		if (ImGui::BeginTabBar("TabVar"))
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				for (auto& pair : SceneDataManager::GetInstanceID2PointerMap())
				{
					ImGui::PushID(SceneEditor::FieldID++);
					if (ImGui::Selectable((pair.second->name + "(" + pair.first + ")").c_str()))
					{
						outHasChanged = true;

						serializedData = serializedVarName + "(instanceID)" + pair.first;
						//ImGui::CloseCurrentPopup();
					}
					ImGui::PopID();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Asset"))
			{
				for (auto& pair : AssetManager::GetInstanceID2PointerMap())
				{
					if (ImGui::Selectable((pair.second->name + "(" + pair.first + ")").c_str()))
					{
						outHasChanged = true;

						serializedData = serializedVarName + "(instanceID)" + pair.first;
						SceneDataManager::GetInstanceID2PointerMap();
						//ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndPopup();
	}

	return outHasChanged;
}

void ImGuiCreator::Create(SerializedClass* serializedObject)
{
	bool hasChanged = false;
	std::vector<std::string> serializedDataVec = serializedObject->Serialize();

	for (int i = 0; i < serializedDataVec.size(); i++)
	{
		std::function<void()> createContents = [&]() -> void {
			if (i >= serializedDataVec.size())
			{
				return;
			}

			std::string& serializedData = serializedDataVec[i];
			std::smatch smatch;
			std::string instanceID = "";

			ImGui::PushID(i);

			// 値
			if (std::regex_match(serializedData, smatch, std::regex(R"((\s*)(\w+):\s(.+))")))
			{
				std::string indent = smatch[1].str();
				std::string label = smatch[2].str();
				std::string value = smatch[3].str();

				std::string serializedVarName = indent + label + ": ";

				if (ArithmeticField(serializedData, serializedVarName, label, value))
				{
					hasChanged = true;
				}
			}
			// クラス、構造体
			else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*(\w+):)")))
			{
				ImGui::Text(serializedData.c_str());

				/*if (ImGui::TreeNode(serializedData.c_str()))
				{
					ImGui::TreePop();
				}*/
			}
			// vector
			else if (std::regex_match(serializedData, smatch, std::regex(R"((\s*\(vector\)(\w+:)\s)(\d+))")))
			{
				std::string serializedVarName = smatch[1].str();
				std::string label = smatch[2].str();
				int size = std::stoi(smatch[3].str());

				// ラベル
				ImGui::Text(label.c_str());

				// 要素数のフィールド
				if (ImGui::InputInt("size", &size))
				{
					hasChanged = true;

					serializedData = serializedVarName + std::to_string(size);
				}
			}
			// vectorの要素
			else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*-\s(.+))")))
			{
				// 要素が値
				if (std::regex_match(serializedData, smatch, std::regex(R"((\s*)-\s(.+))")))
				{
					std::string indent = smatch[1].str();
					std::string label = "-";
					std::string value = smatch[2].str();

					std::string serializedVarName = indent + "- ";

					if (ArithmeticField(serializedData, serializedVarName, label, value))
					{
						hasChanged = true;

						// ここで値の変更処理をした方が統一感がある
					}
				}
				// 要素がポインタ
				else if (IsPointer(instanceID, smatch[1].str()))
				{
					Debug::Log(L"要素がポインタの時の挙動が未定義です。[ImGuiCreator::Create()]");
				}
			}
			else
			{
				ImGui::Text(serializedData.c_str());
			}

			ImGui::PopID();
		};

		createContents();
	}

	if (hasChanged)
	{
		for (std::string& serializedData : serializedDataVec)
		{
			// 空白除去
			std::smatch smatch;
			if (std::regex_match(serializedData, smatch, std::regex(R"(\s{2}(.+))")))
			{
				serializedData = smatch[1].str();
			}
		}

		serializedObject->Deserialize(serializedDataVec);
	}
}

bool ImGuiCreator::IsArithmetic(std::smatch& outSmatch, const std::string& value)
{
	if (std::regex_match(value, outSmatch, std::regex(R"((\s*)(\w+):\s(.+))")))
	{
		return true;
	}
	return false;
}

bool ImGuiCreator::IsPointer(std::string& instanceID, const std::string& value)
{
	std::smatch smatch;
	if (std::regex_match(value, smatch, std::regex(R"(\(\w+\)(.+))")))
	{
		instanceID = smatch[1].str();
		return true;
	}
	instanceID = "nullptr";
	return false;
}

bool ImGuiCreator::ArithmeticField(std::string& outSerializedData, const std::string& serializedVarName, const std::string& label, const std::string& value)
{
	bool outHasChanged = false;

	std::string instanceID;
	std::smatch smatch;

	// int
	if (std::regex_match(value, smatch, std::regex(R"(-?\d+)")))
	{
		int v = std::stoi(value);
		if (ImGui::DragInt(label.c_str(), &v))
		{
			outHasChanged = true;

			outSerializedData = serializedVarName + std::to_string(v);
		}
	}
	// float
	else if (std::regex_match(value, smatch, std::regex(R"(-?\d+\.\d+)")))
	{
		float v = std::stof(value);
		if (ImGui::DragFloat(label.c_str(), &v))
		{
			outHasChanged = true;

			outSerializedData = serializedVarName + std::to_string(v);
		}
	}
	// bool
	else if (value == "true" || value == "false")
	{
		bool b = (value == "true");
		if (ImGui::Checkbox(label.c_str(), &b))
		{
			outHasChanged = true;

			outSerializedData = serializedVarName + (b ? "true" : "false");
		}
	}
	// pointer
	else if (IsPointer(instanceID, value))
	{
		outHasChanged = PutPointerField(outSerializedData, serializedVarName, label, instanceID);
	}
	// 文字列
	else if (std::regex_match(value, smatch, std::regex(R"(.+)")))
	{
		static char buf[256];
		if (ImGui::InputText(label.c_str(), buf, IM_ARRAYSIZE(buf)))
		{
			outHasChanged = true;
			outSerializedData = serializedVarName + std::string(buf);
		}
	}
	else
	{
		Debug::Log(L"型を追加してください。[ArithmeticField()]");
	}

	return outHasChanged;
}