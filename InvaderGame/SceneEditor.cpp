#include "SceneEditor.h"

#include "framework.h"
#include "Utility.h"
#include "GameEngine.h"
#include "ImGuiUtility.h"
#include "GameSystem.h"
#include "SceneDataManager.h"
#include "ImGuiCreator.h"
#include "AnimationEditor.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void SceneEditor::Initialize()
{
	Texture* texture = new Texture();
	texture->Load("Resources/Texture/Frame1.png");
	_frameObject = GameObject::Create();
	_frameObject->AddComponent<SpriteRenderer>()->SetTexture(texture);

	EngineTime::TimeScale = 0;


	// エディタ起動コマンド
	//InputSystem::AddKeyAction({ InputSystem::KeySet('E') }, []() -> void {
	//	_isEditMode = !_isEditMode;

	//	if (_isEditMode)
	//	{
	//		// シーンを再生成する
	//		SceneDataManager::Reload();

	//		EngineTime::TotalTime = 0;
	//		EngineTime::TimeScale = 0;
	//	}
	//	else
	//	{
	//		// ここでRecordを初期化する
	//		for (RecordBase* record : RecordManager::RecordVector)
	//		{
	//			record->Initialize();
	//		}

	//		// TimeScaleで管理すると、無駄な処理がずっと走ることになるから、要検討
	//		EngineTime::TimeScale = 1;
	//	}
	//});


	// 設置フレーム移動コマンド
	/*InputSystem::AddKeyAction({ InputSystem::KeySet('W') }, []() -> void { _frameObject->GetTransform()->position.Get().y += 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('A') }, []() -> void { _frameObject->GetTransform()->position.Get().x -= 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('S') }, []() -> void { _frameObject->GetTransform()->position.Get().y -= 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('D') }, []() -> void { _frameObject->GetTransform()->position.Get().x += 1; });*/

	// 設置コマンド
	/*InputSystem::AddKeyAction({ InputSystem::KeySet('J') }, []() -> void {
		if (!_isEditMode) return;
		GameObject* gameObject = GameObject::Create();
		gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
		gameObject->AddComponent<SpriteRenderer>();

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(gameObject);
	});*/

	// セーブコマンド
	InputSystem::AddKeyAction({ InputSystem::KeySet('P') }, []() -> void { SceneDataManager::Save(); });
}

void SceneEditor::Update()
{
	static bool isAssetBrowserOpen = false;

	// シーンエディタの切り替え
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('E'))
	{
		_isEditMode = !_isEditMode;

		if (_isEditMode)
		{
			// シーンを再生成する
			SceneDataManager::Reload();

			EngineTime::TotalTime = 0;
			EngineTime::TimeScale = 0;
		}
		else
		{
			// ここでRecordを初期化する
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Initialize();
			}

			// TimeScaleで管理すると、無駄な処理がずっと走ることになるから、要検討
			EngineTime::TimeScale = 1;
		}
	}

	if (!_isEditMode) return;

	// 設置モード切り替え
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('O'))
	{
		_isPuttingMode = !_isPuttingMode;
	}

	// 他エディタのUpdate処理
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
			if (ImGui::Selectable((gameObjectVector[i]->name + std::to_string(i)).c_str(), selected == i))
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
					Object::Destroy(gameObjectVector[i]);
				}
				if (ImGui::Selectable("Create Prefab"))
				{
					std::string serializedData = "";
					GameObject* gameObject = gameObjectVector[i];
					AssetManager::SerializeGameObject(serializedData, gameObject);
					FileManager::Write("Resources/Prefab/" + gameObject->name + ".prefab", serializedData);
				}
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();


	// インスペクター
	ImGui::SetNextWindowPos(ImVec2(210, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 500));
	ImGui::Begin("Inspector");
	if (Selection::gameObject != nullptr)
	{
		// Componentの配置
		for (auto& component : Selection::gameObject->GetComponentVector())
		{
			// Component
			if (ImGui::CollapsingHeader(component.get()->GetName().c_str()))
			{
				ImGuiCreator::Create(component.get());
			}

			// Remove Component
			if (ImGui::BeginPopupContextItem())
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
				//GameSystem::GetInstance().AddDelayedExecution([&]() -> void { Selection::gameObject->AddComponent(component); });
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();


	// PuttingMode
	if (!_isPuttingMode)
	{
		return;
	}

	// フレームの移動
	if (Input::GetKeyDown('W')) _frameObject->GetTransform()->position.Get().y += 1;
	else if (Input::GetKeyDown('A')) _frameObject->GetTransform()->position.Get().x -= 1;
	else if (Input::GetKeyDown('S')) _frameObject->GetTransform()->position.Get().y -= 1;
	else if (Input::GetKeyDown('D')) _frameObject->GetTransform()->position.Get().x += 1;

	// 設置
	if (Input::GetKeyDown('J'))
	{
		GameObject* gameObject = GameObject::Create();
		gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
		gameObject->AddComponent<SpriteRenderer>();

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(gameObject);
	}

	// フレームの描画
	_frameObject->Update();
}

int SceneEditor::FieldID = 0;
bool SceneEditor::_isEditMode = true;
bool SceneEditor::_isPuttingMode = false;
GameObject* SceneEditor::_frameObject = nullptr;
