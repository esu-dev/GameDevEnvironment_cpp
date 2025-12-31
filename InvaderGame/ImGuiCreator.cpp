#include "ImGuiCreator.h"

#include "imgui_impl_dx11.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
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
	ImGui::SetNextWindowSize(ImVec2(0, 600));
	if (ImGui::BeginPopup("select_instanceID_popup"))
	{
		if (ImGui::BeginTabBar("TabVar"))
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				// Noneの配置
				if (ImGui::Selectable("none"))
				{
					outHasChanged = true;

					serializedData = serializedVarName + "nullptr";
				}

				std::function<void(GameObject*)> putSceneInstance = [&](GameObject* go) -> void {
						// 親TreeNode
						ImGui::PushID(go->instanceID.c_str());
						bool isTreeOpen = ImGui::TreeNodeEx(go->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
						if (ImGui::IsItemClicked())
						{
							outHasChanged = true;

							serializedData = serializedVarName + "(instanceID)" + go->instanceID;
						}
						ImGui::PopID();

						if (isTreeOpen)
						{
							// コンポーネントを列挙
							for (auto& component : go->GetComponentVector())
							{
								std::string componentID = component.get()->instanceID;
								if (ImGui::Selectable((component.get()->GetName() + " (" + componentID + ")").c_str()))
								{
									outHasChanged = true;

									serializedData = serializedVarName + "(instanceID)" + componentID;
								}
							}

							// 子オブジェクトに対して再帰的に実行
							for (auto& child : go->GetTransform()->GetChildVector())
							{
								putSceneInstance(child->gameObject);
							}

							ImGui::TreePop();
						}
					};


				for (auto& pair : SceneDataManager::GetInstanceID2PointerMap())
				{
					if (pair.second == nullptr)
					{
						continue;
					}

					if (GameObject* gameObject = dynamic_cast<GameObject*>(pair.second))
					{
						if (gameObject->GetTransform()->GetParent() != nullptr)
						{
							continue;
						}
						putSceneInstance(gameObject);
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Asset"))
			{
				if (ImGui::Selectable("none"))
				{
					outHasChanged = true;

					serializedData = serializedVarName + "nullptr";
				}

				std::function<void(AssetManager::AssetFolder)> createAssetGui = [&](const AssetManager::AssetFolder& assetFolder) -> void {
					for (auto& pair : assetFolder.name2Datamp)
					{
						std::string folderName = pair.first;
						auto assetData = pair.second;

						// Folder
						if (std::holds_alternative<AssetManager::AssetFolder*>(assetData))
						{
							if (ImGui::TreeNode(folderName.c_str()))
							{
								auto childAssetFolder = *std::get<AssetManager::AssetFolder*>(assetData);
								createAssetGui(childAssetFolder);
								ImGui::TreePop();
							}
						}
						// AssetFile
						else
						{
							auto assetFile = std::get<AssetManager::AssetFile*>(assetData);
							if (ImGui::Selectable((folderName + "  (" + assetFile->instanceID + ")").c_str()))
							{
								outHasChanged = true;

								serializedData = serializedVarName + "(instanceID)" + assetFile->instanceID;
							}
						}
					}
				};

				createAssetGui(AssetManager::GetAssetFolder());

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
				if (std::regex_match(serializedData, smatch, std::regex(R"((\s*)-\s([^:]+))")))
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
				// 要素がクラス，構造体
				else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*-\s(\w+):)")))
				{
					std::string label = smatch[1].str();

					ImGui::Text(label.c_str());
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

		// OnValueChangedイベントを発行
		// 現状はComponentのみ対応
		if (Component* component = dynamic_cast<Component*>(serializedObject))
		{
			component->OnValidate();

			// 親要素もOnValidateを呼んで更新する
		}
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
	else if (std::regex_match(value, smatch, std::regex(R"(\"(.*)\")")))
	{
		std::string string = smatch[1].str();
		static char buf[256];
		strcpy_s(buf, string.c_str());
		if (ImGui::InputText(label.c_str(), buf, IM_ARRAYSIZE(buf)))
		{
			outHasChanged = true;
			outSerializedData = serializedVarName + "\"" + std::string(buf) + "\"";
		}
	}
	else
	{
		Debug::Log(L"型を追加してください。[ArithmeticField()]");
	}

	return outHasChanged;
}