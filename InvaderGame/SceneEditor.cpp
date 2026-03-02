#include "SceneEditor.h"

#include "framework.h"
#include "Utility.h"
#include "GameEngine.h"
#include "ImGuiUtility.h"
#include "GameSystem.h"
#include "SceneDataManager.h"
#include "ImGuiCreator.h"
#include "LevelEditor.h"
#include "AnimationEditor.h"
#include "TimeController.h"
#include "AssetExplorer.h"
#include "EditorCamera.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

using namespace SceneManagement;


bool SceneEditor::GetIsEditMode()
{
	return _isEditMode;
}

void SceneEditor::Initialize()
{
	_focusFrame = GameObject::Create();
	_focusFrame->AddComponent<SpriteRenderer>()->SetTexture(new Texture("Resources/Texture/Frame3.png"));
	_focusFrame->GetComponent<SpriteRenderer>()->SetOrder(100);

	EngineTime::TimeScale = 0;

	// 他のエディタの初期化処理
	LevelEditor::Initialize();
	EditorCamera::Start();
}

void SceneEditor::Update()
{
	static bool isAssetBrowserOpen = false;
	static bool isShowingWindow = true;

	// シーンエディタの切り替え
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('E'))
	{
		_isEditMode = !_isEditMode;

		// シーンを再生成する
		SceneDataManager::Reload();

		if (_isEditMode)
		{
			EngineTime::SetTotalTime(0);
			EngineTime::TimeScale = 0;

			Selection::gameObject = nullptr;

			TimeController::Initialize();
		}
		else
		{
			// ここでRecordを初期化する
			// 初期化タイミングをStart()以降にしないといけないのでは？
			// Start()で設定されなかった変数を初期化する
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Initialize();
			}

			// TimeScaleで管理すると、無駄な処理がずっと走ることになるから、要検討
			EngineTime::TimeScale = 1;
			EngineTime::ResetDeltaTime();
		}
	}


	// エディタを開いていないならば
	if (!_isEditMode)
	{
		TimeController::Update();

		return;
	}


	// 以下エディタ中の処理 ---------------------------

	// レコードシーンを読み込む
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('R'))
	{
		Debug::Log("RecordSceneの読み込みを開始しました。");

		_isEditMode = false;

		SceneDataManager::LoadRecord();

		// TimeScaleで管理すると、無駄な処理がずっと走ることになるから、要検討
		EngineTime::TimeScale = 1;

		// TotalTimeの復元
		std::vector<std::string> contentVector;
		FileManager::Read(contentVector, "Resources/Scenes/" + SceneManager::GetActiveScene()->GetName() + "_time.txt");
		EngineTime::SetTotalTime(std::stof(contentVector[0]));
		EngineTime::ResetDeltaTime();
		Debug::Log("RecordSceneの読み込みが完了しました。");
		//Debug::Log("TotalTimeを%sに設定しました。", contentVector[0].c_str());
		Debug::Log("TotalTime: %f", EngineTime::GetTotalTime());
	}


	// 非表示にする
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown(VK_SPACE))
	{
		isShowingWindow = !isShowingWindow;
	}

	if (!isShowingWindow) return;


	// 他エディタのUpdate処理
	LevelEditor::Update();
	AnimationEditor::Update();
	AssetExplorer::Update();
	EditorCamera::Update();

	// imguiデモ表示
	//ImGui::ShowDemoWindow();


	// ヒエラルキー
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImVec2(200, 500));
	//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.25f);
	ImGui::Begin("Hierarchy");

	// シーン名の配置
	static char buf[64];
	strcpy_s(buf, SceneManager::GetActiveScene()->GetName().c_str());
	if (ImGui::InputText("SceneName", buf, IM_ARRAYSIZE(buf)))
	{
		std::string newName = std::string(buf);
		SceneManager::GetActiveScene()->SetName(newName);
	}

	// 展開されている状態のときTrueを返すから、入れ子が実現できる
	if (ImGui::CollapsingHeader(SceneManager::GetActiveScene()->GetName().c_str()))
	{
		static int selected = -1;

		auto putPopup = [](GameObject* go) -> void {
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Selectable("Duplicate"))
				{
					GameObject* newGameObject = Object::Instantiate(go);
					//SceneManager::GetActiveScene()->AddGameObject(newGameObject); Instantiate()で既にシーンに追加されている。
				}
				if (ImGui::Selectable("Delete"))
				{
					// 選択を外す
					Selection::gameObject = nullptr;

					// Sceneのインスタンスから除外する
					SceneDataManager::GetInstanceID2PointerMap().erase(go->instanceID);

					for (auto component : go->GetComponentVector())
					{
						SceneDataManager::GetInstanceID2PointerMap().erase(component.get()->instanceID);
					}

					Object::Destroy(go);
				}
				if (ImGui::Selectable("Create Prefab"))
				{
					// ゲームオブジェクトのシリアライズ
					// 子要素も行う必要がある
					std::string serializedData = "";
					GameObject* gameObject = go;
					gameObject->IsPrefab = true;

					// 再帰的にシリアライズ
					std::function<void(GameObject*)> serializeGameObject_Recursively = [&](GameObject* targetGameObject) -> void {
						AssetManager::SerializeGameObject(serializedData, targetGameObject);

						// 子要素のシリアライズ
						for (auto transform : targetGameObject->GetTransform()->GetChildVector())
						{
							serializeGameObject_Recursively(transform->gameObject);
						}
						};

					serializeGameObject_Recursively(gameObject);


					std::string directry = "Resources/Prefab/";
					std::string fileName = gameObject->name + ".prefab";
					FileManager::Write(directry + fileName, serializedData);
					AssetManager::InstantiateAsset(directry, fileName, SceneDataManager::GetInstanceID2PointerMap());

					//Object::Destroy(gameObject);
				}
				ImGui::EndPopup();
			}
		};

		// 子要素も含めたGameObjectの配置
		std::function<void(int, int, GameObject*)> putGameObject = [&](int id, int depth, GameObject* go) -> void {
			// 深さ０で親がいるなら配置しない
			if (depth == 0 && go->GetTransform()->GetParent() != nullptr)
			{
				return;
			}
			
			// 同じ名前はIDが同じになってしまう。IDを付与する必要がある。
			ImGui::PushID(id);

			bool isActive = go->ActiveSelf() && (go->GetTransform()->GetParent() == nullptr || go->GetTransform()->GetParent()->gameObject->ActiveSelf());

			bool isTreeOpen = false;
			bool isSelected = (selected == id);

			// 子要素があるならTreeNodeExを使う
			if (go->GetTransform()->GetChildVector().size() > 0)
			{
				// 矢印をクリックしたときに展開するようにする
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
				if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;

				// 親TreeNode
				if (!isActive) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
				isTreeOpen = ImGui::TreeNodeEx(go->name.c_str(), flags);
				if (!isActive) ImGui::PopStyleColor();
				
				if (ImGui::IsItemClicked())
				{
					selected = id;
					Selection::gameObject = go;
				}
			}
			else
			{
				ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
				if (!isActive) ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
				if (ImGui::Selectable(go->name.c_str(), isSelected))
				{
					selected = id;
					Selection::gameObject = go;
				}
				if (!isActive) ImGui::PopStyleColor();
				ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
			}

			// ドラッグ＆ドロップの処理
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("GAMEOBJECT_PTR", &go, sizeof(GameObject*));
				ImGui::Text("%s", go->name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_PTR"))
				{
					GameObject* draggedGo = *(GameObject**)payload->Data;
					if (draggedGo != go)
					{
						// 循環参照のチェック（goがdraggedGoの子孫でないか）
						bool isDescendant = false;
						Transform* checkParent = go->GetTransform()->GetParent();
						while (checkParent != nullptr)
						{
							if (checkParent->gameObject == draggedGo)
							{
								isDescendant = true;
								break;
							}
							checkParent = checkParent->GetParent();
						}

						if (!isDescendant)
						{
							draggedGo->GetTransform()->SetParent(go->GetTransform());
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			putPopup(go);

			if (isTreeOpen)
			{
				// 再帰的に子要素を配置
				int childID = 0;
				for (auto& child : go->GetTransform()->GetChildVector())
				{
					putGameObject(std::stoi(std::to_string(id) + "0") + childID++, depth + 1, child->gameObject);
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		};

		auto gameObjectVector = SceneManagement::SceneManager::GetActiveScene()->GetGameObjectVector();
		for (int i = 0; i < (int)gameObjectVector.size(); i++)
		{
			putGameObject(i, 0, gameObjectVector[i]);
		}

		// 何もないところへのドロップで親を解除
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "(Drop here to unparent)");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_PTR"))
			{
				GameObject* draggedGo = *(GameObject**)payload->Data;
				draggedGo->GetTransform()->SetParent(nullptr);
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();


	// インスペクター
	//ImGui::SetNextWindowPos(ImVec2(210, 0));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Inspector");
	if (Selection::gameObject != nullptr)
	{
		// Active
		bool isActive = Selection::gameObject->ActiveSelf();
		if (ImGui::Checkbox("Active", &isActive))
		{
			Selection::gameObject->SetActive(isActive);
		}

		// オブジェクト名の配置
		static char buf[64];
		strcpy_s(buf, Selection::gameObject->name.c_str());
		if (ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf)))
		{
			Selection::gameObject->name = std::string(buf);
		}

		// タグ
		static char tagBuf[64];
		strcpy_s(tagBuf, Selection::gameObject->tag.c_str());
		if (ImGui::InputText("Tag", tagBuf, IM_ARRAYSIZE(tagBuf)))
		{
			Selection::gameObject->tag = std::string(tagBuf);
		}

		// Componentの配置
		for (auto& component : Selection::gameObject->GetComponentVector())
		{
			// Component
			if (ImGui::CollapsingHeader(component.get()->GetName().c_str()))
			{
				ImGuiCreator::Create(component.get());
			}
			// Remove Component
			else if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Selectable("Remove Component"))
				{
					Selection::gameObject->RemoveComponent(component);
				}
				ImGui::EndPopup();
			}
		}

		// AddComponent
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("add_component_popup");
		}
		if (ImGui::BeginPopup("add_component_popup"))
		{
			std::vector<std::string> componentNameVec = Activator::GetObjectNameVec();
			static int selectedIndex = 0;
			const char* comboPreviewValue = componentNameVec[selectedIndex].c_str();
			if (ImGui::BeginCombo("-", comboPreviewValue))
			{
				for (int n = 0; n < componentNameVec.size(); n++)
				{
					bool isSelected = (n == selectedIndex);
					if (ImGui::Selectable(componentNameVec[n].c_str(), isSelected)) selectedIndex = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected)	ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Add Component"))
			{
				Component* component = dynamic_cast<Component*>(Activator::CreateInstance(componentNameVec[selectedIndex]));
				Selection::gameObject->AddComponent(component);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
	//ImGui::PopStyleVar();


	// ギズモ操作の切替
	if (!Input::GetKey(VK_CONTROL))
	{
		if (Input::GetKeyDown('T')) _gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		if (Input::GetKeyDown('R')) _gizmoOperation = ImGuizmo::OPERATION::ROTATE;
		if (Input::GetKeyDown('S')) _gizmoOperation = ImGuizmo::OPERATION::SCALE;
	}

	// ImGuizmo
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(true);
	ImGuizmo::SetRect(0, 0, GameSystem::WINDOW_WIDTH, GameSystem::WINDOW_HEIGHT);
	if (Selection::gameObject != nullptr)
	{
		float viewMat[16];
		float projMat[16];
		float worldMat[16];
		EditorCamera::GetViewMatrix(viewMat);
		EditorCamera::GetProjMatrix(projMat);
		Selection::gameObject->GetTransform()->GetWorldMatrix(worldMat);

		if (ImGuizmo::Manipulate(
			viewMat,
			projMat,
			(ImGuizmo::OPERATION)_gizmoOperation,
			ImGuizmo::MODE::LOCAL,
			worldMat
		))
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(worldMat, matrixTranslation, matrixRotation, matrixScale);

			Selection::gameObject->GetTransform()->position = Vector3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
			Selection::gameObject->GetTransform()->rotation = Quaternion::Euler(Vector3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));
			Selection::gameObject->GetTransform()->scale = Vector3(matrixScale[0], matrixScale[1], matrixScale[2]);
		}
	}


	// セーブ
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('S'))
	{
		SceneDataManager::Save();
	}
}

int SceneEditor::FieldID = 0;
bool SceneEditor::_isEditMode = true;
GameObject* SceneEditor::_focusFrame = nullptr;
int SceneEditor::_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
