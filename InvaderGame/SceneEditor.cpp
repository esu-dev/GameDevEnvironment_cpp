#include "SceneEditor.h"

#include "framework.h"
#include "Utility.h"
#include "GameEngine.h"
#include "ImGuiUtility.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void SceneEditor::Initialize()
{
	Texture* texture = new Texture("Resources/Texture/Frame1.png");
	_frameObject = GameObject::Create();
	_frameObject->AddComponent<SpriteRenderer>()->SetTexture(texture);


	// 設置フレーム移動コマンド
	InputSystem::AddKeyAction({ InputSystem::KeySet('W') }, []() -> void { _frameObject->GetTransform()->position.Get().y += 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('A') }, []() -> void { _frameObject->GetTransform()->position.Get().x -= 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('S') }, []() -> void { _frameObject->GetTransform()->position.Get().y -= 1; });
	InputSystem::AddKeyAction({ InputSystem::KeySet('D') }, []() -> void { _frameObject->GetTransform()->position.Get().x += 1; });

	// 設置コマンド
	InputSystem::AddKeyAction({ InputSystem::KeySet('J') }, []() -> void {
		GameObject* gameObject = GameObject::Create();
		gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
		gameObject->AddComponent<SpriteRenderer>();

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(gameObject);
	});

	// セーブコマンド
	InputSystem::AddKeyAction({ InputSystem::KeySet('P') }, []() -> void { SceneDataManager::Save(); });
}

void SceneEditor::Update()
{
	// imgui表示
	ImGuiUtility::BeginFrame();
	ImGui::ShowDemoWindow();
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
			// 同じ名前はIDが同じになってしまう。IDを付与する必要がある。
			if (ImGui::Selectable((gameObjectVector[i]->name + std::to_string(i)).c_str(), selected == i))
			{
				selected = i;

				Selection::gameObject = gameObjectVector[i];
			}
		}
	}

	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(300, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("Inspector");
	if (Selection::gameObject != nullptr)
	{
		for (auto& component : Selection::gameObject->GetComponentVector())
		{
			if (ImGui::CollapsingHeader(component.get()->GetName().c_str()))
			{

			}
		}
	}
	ImGui::End();

	// 
	_frameObject->Update();
}

GameObject* SceneEditor::_frameObject = nullptr;