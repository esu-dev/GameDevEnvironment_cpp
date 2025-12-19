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

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void SceneEditor::Initialize()
{
	_focusFrame = GameObject::Create();
	_focusFrame->AddComponent<SpriteRenderer>()->SetTexture(new Texture("Resources/Texture/Frame3.png"));

	EngineTime::TimeScale = 0;

	// 他のエディタの初期化処理
	LevelEditor::Initialize();
}

void SceneEditor::Update()
{
	static bool isAssetBrowserOpen = false;

	// シーンエディタの切り替え
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('E'))
	{
		_isEditMode = !_isEditMode;

		// シーンを再生成する
		SceneDataManager::Reload();

		if (_isEditMode)
		{
			EngineTime::TotalTime = 0;
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
		}
	}

	// エディタを開いていないならば
	if (!_isEditMode)
	{
		TimeController::Update();

		return;
	}

	

	// 他エディタのUpdate処理
	LevelEditor::Update();
	AnimationEditor::Update();

	// imguiデモ表示
	ImGui::ShowDemoWindow();


	// ヒエラルキー
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 500));
	ImGui::Begin("Hierarchy");

	// 展開されている状態のときTrueを返すから、入れ子が実現できる
	if (ImGui::CollapsingHeader("SceneName"))
	{
		static int selected = -1;

		auto gameObjectVector = SceneManagement::SceneManager::GetActiveScene()->GetGameObjectVector();
		for (int i = 0; i < gameObjectVector.size(); i++)
		{
			// GameOBject
			// 同じ名前はIDが同じになってしまう。IDを付与する必要がある。
			ImGui::PushID(i);
			if (ImGui::Selectable((gameObjectVector[i]->name).c_str(), selected == i))
			{
				selected = i;

				Selection::gameObject = gameObjectVector[i];
			}
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Selectable("Duplicate"))
				{
					//SceneManagement::SceneManager::GetActiveScene()->AddGameObject()
				}
				if (ImGui::Selectable("Delete"))
				{
					// 選択を外す
					Selection::gameObject = nullptr;

					Object::Destroy(gameObjectVector[i]);
				}
				if (ImGui::Selectable("Create Prefab"))
				{
					std::string serializedData = "";
					GameObject* gameObject = gameObjectVector[i];
					gameObject->IsPrefab = true;
					AssetManager::SerializeGameObject(serializedData, gameObject);
					
					std::string directry = "Resources/Prefab/";
					std::string fileName = gameObject->name + ".prefab";
					FileManager::Write(directry + fileName, serializedData);
					AssetManager::CreateInstance(directry, fileName);

					//Object::Destroy(gameObject);
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
	}
	ImGui::End();


	// インスペクター
	ImGui::SetNextWindowPos(ImVec2(210, 0));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Inspector");
	if (Selection::gameObject != nullptr)
	{
		// オブジェクト名の配置
		static char buf[64];
		strcpy_s(buf, Selection::gameObject->name.c_str());
		if (ImGui::InputText("Name", buf, IM_ARRAYSIZE(buf)))
		{
			Selection::gameObject->name = std::string(buf);
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


	// フォーカスフレーム
	if (Selection::gameObject != nullptr)
	{
		_focusFrame->GetTransform()->position = Selection::gameObject->GetTransform()->position;
		_focusFrame->GetTransform()->scale = Selection::gameObject->GetTransform()->scale + Vector3::one;
		_focusFrame->Update();
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
