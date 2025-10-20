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
	InputSystem::KeySet keyset_ctrl;
	keyset_ctrl .isHold = true;
	keyset_ctrl.vkey = VK_CONTROL;

	InputSystem::KeySet keyset_s;
	keyset_s.isHold = false;
	keyset_s.vkey = 'S';

	InputSystem::AddKeyAction({ keyset_ctrl, keyset_s }, []() -> void { SceneDataManager::Save(); });
}

void SceneEditor::Update()
{
	// imgui表示
	/*ImGui::SetNextWindowSize(ImVec2(320, 100));
	ImGui::Begin("hoge");
	ImGui::Text("fugafuga");
	ImGui::End();*/
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
			}
		}
	}

	ImGui::End();

	if (Input::GetKeyDown('A'))
	{
		GameObject* gameObject = GameObject::Create();
		gameObject->AddComponent<SpriteRenderer>();

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(gameObject);
	}
}