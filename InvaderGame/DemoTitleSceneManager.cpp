#include "DemoTitleSceneManager.h"

#include "Input.h"
#include "SceneManager.h"

void DemoTitleSceneManager::Update()
{
	if (Input::GetKeyDown(VK_SPACE))
	{
		// ƒV[ƒ“‘JˆÚ
		SceneManagement::SceneManager::LoadSceneAsset("DemoGameScene");
	}
}