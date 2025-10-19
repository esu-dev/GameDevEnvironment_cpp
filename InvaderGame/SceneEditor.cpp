#include "SceneEditor.h"

#include "framework.h"
#include "Utility.h"
#include "GameEngine.h"

#include "imgui.h"

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
	// imgui•\Ž¦
	/*ImGui::SetNextWindowSize(ImVec2(320, 100));
	ImGui::Begin("hoge");
	ImGui::Text("fugafuga");
	ImGui::End();*/

	if (Input::GetKeyDown('A'))
	{
		GameObject* gameObject = GameObject::Create();
		gameObject->AddComponent<SpriteRenderer>();

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(gameObject);
	}
}