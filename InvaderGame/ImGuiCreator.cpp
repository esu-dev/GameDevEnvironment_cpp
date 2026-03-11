#include "ImGuiCreator.h"

#include "EngineFramework.h"
#include "imgui_impl_dx11.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "SceneEditor.h"
#include "SceneDataManager.h"
#include "AnimationClip.h"


void ImGuiCreator::CreateAssetGui(const AssetManager::AssetFolder& assetFolder, const std::function<void(const std::string& assetName, AssetManager::AssetFile* assetFile)>& selectedAction)
{
	// 1. まずフォルダをツリー形式で表示
	for (auto& pair : assetFolder.name2Datamp)
	{
		std::string folderName = pair.first;
		auto assetData = pair.second;

		if (std::holds_alternative<AssetManager::AssetFolder*>(assetData))
		{
			if (ImGui::TreeNode(folderName.c_str()))
			{
				auto childAssetFolder = *std::get<AssetManager::AssetFolder*>(assetData);
				CreateAssetGui(childAssetFolder, selectedAction);
				ImGui::TreePop();
			}
		}
	}

	// 2. 次にファイルをアイテム（アイコン）形式でグリッド表示
	float thumbnailSize = 64.0f;
	float padding = 16.0f;
	float cellSize = thumbnailSize + padding;

	float windowWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(windowWidth / cellSize);
	if (columnCount < 1) columnCount = 1;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding * 0.5f, padding * 0.5f));
	if (ImGui::BeginTable("AssetIconGrid", columnCount))
	{
		for (auto& pair : assetFolder.name2Datamp)
		{
			std::string assetName = pair.first;
			auto assetData = pair.second;

			if (std::holds_alternative<AssetManager::AssetFile*>(assetData))
			{
				ImGui::TableNextColumn();
				auto assetFile = std::get<AssetManager::AssetFile*>(assetData);

				ImGui::PushID(assetName.c_str());
				ImGui::BeginGroup();

				// アイコンの背景色をファイルタイプ別に設定（Unityの風の色分け）
				ImU32 iconColor = IM_COL32(80, 80, 80, 255); // デフォルト：ダークグレー
				if (assetFile->object) {
					std::string typeName = assetFile->object->GetName();
					if (typeName == "Texture") iconColor = IM_COL32(50, 100, 200, 255);      // テクスチャ：青
					else if (typeName == "AudioClip") iconColor = IM_COL32(50, 200, 100, 255);   // オーディオ：緑
					else if (typeName == "GameObject") iconColor = IM_COL32(100, 150, 255, 255); // プレハブ：水色
					else if (typeName == "SceneAsset") iconColor = IM_COL32(200, 100, 100, 255); // シーン：赤
					else if (typeName == "Mesh") iconColor = IM_COL32(150, 150, 150, 255);       // メッシュ：明るいグレー
					else if (typeName == "AnimationClip") iconColor = IM_COL32(255, 150, 50, 255); // アニメーション
				}

				
				// アイコンボタン
				ImVec2 cursorPos = ImGui::GetCursorPos();

				// Texture または AnimationClip の場合は画像をボタンに割り当てる
				Texture* displayTexture = nullptr;
				if (assetFile->object != nullptr)
				{
					if (assetFile->object->GetName() == "Texture")
					{
						displayTexture = dynamic_cast<Texture*>(assetFile->object);
					}
					else if (assetFile->object->GetName() == "AnimationClip")
					{
						AnimationClip* clip = dynamic_cast<AnimationClip*>(assetFile->object);
						if (clip && !clip->GetAnimDataSetVec().empty())
						{
							displayTexture = clip->GetAnimDataSetVec()[0].texture;
						}
					}
				}

				if (displayTexture != nullptr)
				{
					if (ImGui::ImageButton("##", (ImTextureID)displayTexture->m_shaderResourceview.Get(), ImVec2(thumbnailSize, thumbnailSize)))
					{
						selectedAction(assetName, assetFile);
					}
				}
				else
				{
					if (ImGui::Button("##", ImVec2(thumbnailSize, thumbnailSize)))
					{
						selectedAction(assetName, assetFile);
					}

					// アイコンの装飾（色付き矩形と枠線）
					ImDrawList* drawList = ImGui::GetWindowDrawList();
					ImVec2 screenPos = ImGui::GetItemRectMin();
					ImVec2 screenMax = ImGui::GetItemRectMax();
					drawList->AddRectFilled(ImVec2(screenPos.x + 4, screenPos.y + 4), ImVec2(screenMax.x - 4, screenMax.y - 4), iconColor, 5.0f);
					drawList->AddRect(ImVec2(screenPos.x + 4, screenPos.y + 4), ImVec2(screenMax.x - 4, screenMax.y - 4), IM_COL32(255, 255, 255, 100), 5.0f, 0, 1.0f);
				}

				// 名前ラベル（折り返し表示）
				ImGui::SetCursorPosX(cursorPos.x);
				ImGui::PushTextWrapPos(cursorPos.x + thumbnailSize);
				ImGui::TextWrapped("%s", assetName.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();

				// ホバー時に詳細（インスタンスID）を表示
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("%s\n(ID: %s)", assetName.c_str(), assetFile->instanceID.c_str());
				}

				ImGui::PopID();
			}
		}
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
}

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
								if (ImGui::Selectable((std::string(component.get()->GetName()) + " (" + componentID + ")").c_str()))
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

				CreateAssetGui(AssetManager::GetAssetFolder(), [&](const std::string& assetName, AssetManager::AssetFile* assetFile) -> void {
						outHasChanged = true;

						serializedData = serializedVarName + "(instanceID)" + assetFile->instanceID;
					});

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
		static std::string vectorAttributes = "";

		std::function<void()> createContents = [&]() -> void {
			if (i >= serializedDataVec.size())
			{
				return;
			}

			std::string& serializedData = serializedDataVec[i];
			std::smatch smatch;
			std::string instanceID = "";

			ImGui::PushID(i);

			auto extractAttribute = [](std::string attributes) -> std::vector<std::string> {
				std::vector<std::string> attributeVec;

				// 属性の抽出
				std::regex regex(R"(\w+)");
				std::sregex_iterator iterator(attributes.begin(), attributes.end(), regex);
				std::sregex_iterator end;
				while (iterator != end)
				{
					attributeVec.push_back(iterator->str());
					iterator++;
				}

				return attributeVec;
			};

			auto isHideInspector = [&](std::string attributes) -> bool {
				for (std::string& attribute : extractAttribute(attributes))
				{
					if (attribute == HIDE_INSPECTOR)
					{
						return true;
					}
				}
				return false;
			};

			// 値
			if (std::regex_match(serializedData, smatch, std::regex(R"((\s*)([\w_]+)(\[\w+\])?:\s(.+))")))
			{
				std::string indent = smatch[1].str();
				std::string label = smatch[2].str();
				std::string attributes = smatch[3].str();
				std::string value = smatch[4].str();

				if (isHideInspector(attributes))
				{
					goto skipCreateField;
				}

				std::string serializedVarName = indent + label + ": ";

				if (ArithmeticField(serializedData, serializedVarName, label, value))
				{
					hasChanged = true;
				}
			}
			// クラス、構造体
			else if (std::regex_match(serializedData, smatch, std::regex(R"(\s*(\w+)(\[\w+\])?:)")))
			{
				std::string label = smatch[1].str();
				std::string attributes = smatch[2].str();
				if (isHideInspector(attributes))
				{
					goto skipCreateField;
				}

				ImGui::Text(serializedData.c_str());
			}
			// vector
			else if (std::regex_match(serializedData, smatch, std::regex(R"((\s*\(vector\)(\w+)(\[\w+\])?:\s)(\d+))")))
			{
				std::string serializedVarName = smatch[1].str();
				std::string label = smatch[2].str();
				std::string attributes = smatch[3].str();
				int size = std::stoi(smatch[4].str());

				vectorAttributes = attributes;
				if (isHideInspector(attributes))
				{
					goto skipCreateField;
				}

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
				if (isHideInspector(vectorAttributes))
				{
					goto skipCreateField;
				}

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
skipCreateField:
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