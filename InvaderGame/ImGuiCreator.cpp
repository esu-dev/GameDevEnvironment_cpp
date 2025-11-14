#include "ImGuiCreator.h"

#include "imgui_impl_dx11.h"
#include "Object.h"
#include "SceneEditor.h"
#include "SceneDataManager.h"

void ImGuiCreator::Create(bool& outHasChanged, std::vector<std::string>& serializedDataVec)
{
	static bool _isTreeOpen = false;

	for (int i = 0; i < serializedDataVec.size(); i++)
	{
		std::function<void()> createContents = [&]() -> void {
			if (i >= serializedDataVec.size()) return;

			std::string& serializedData = serializedDataVec[i];
			std::smatch smatch;

			// 空白２個なら木構造終了
			if (_isTreeOpen && std::regex_match(serializedData, smatch, std::regex(R"(\s{2}\w+:.+)")))
			{
				i--;
				_isTreeOpen = false;
				return;
			}

			// 空白が多いならツリーが開いているときのみ
			/*if (!_isTreeOpen && std::regex_match(serializedData, smatch, std::regex(R"(\s{2}\s+\w+:.*)")))
			{
				return;
			}*/

			std::string instanceID = "";

			ImGui::PushID(i);

			// 値
			if (std::regex_match(serializedData, smatch, std::regex(R"((\s*(\w+):\s)(.+))")))
			{
				std::string serializedVarName = smatch[1].str();
				std::string label = smatch[2].str();
				std::string value = smatch[3].str();


				// float
				if (std::regex_match(value, smatch, std::regex(R"(-?\d+\.\d+)")))
				{
					float v = std::stof(value);
					if (ImGui::DragFloat(label.c_str(), &v))
					{
						outHasChanged = true;

						serializedData = serializedVarName + std::to_string(v);
					}
				}
				// bool
				else if (value == "true" || value == "false")
				{
					bool b = (value == "true");
					if (ImGui::Checkbox(label.c_str(), &b))
					{
						outHasChanged = true;

						serializedData = serializedVarName + (b ? "true" : "false");
					}
				}
				// pointer
				else if (IsPointer(instanceID, value))
				{
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
				}
				else
				{
					// これがあるとエラーが出る
					//ImGui::Text(serializedData.c_str());
				}
			}
			// クラス、構造体
			else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*(\w+):)")))
			{
				ImGui::Text(serializedData.c_str());
			}
			// リスト
			else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*-\s(.+))")))
			{
				// 要素がポインタ
				if (IsPointer(instanceID, smatch[1].str()))
				{

				}
			}
			else
			{
				ImGui::Text(serializedData.c_str());
			}

			ImGui::PopID();

			if (_isTreeOpen)
			{
				i++;
				createContents();
			}
			};

		createContents();
	}

	
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

void ImGuiCreator::PutPointerField()
{

}