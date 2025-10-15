#include "SceneManager.h"

#include "Utility.h"
#include "GameEngine.h"

namespace SceneManagement
{
	// static変数の定義
	std::vector<Scene*> SceneManager::_sceneVector;
	Scene* SceneManager::_activeScene;

	Scene* SceneManager::CreateScene(std::string sceneName)
	{
		Scene* newScene = new Scene(sceneName);

		GameObject* camera = GameObject::Create();
		camera->AddComponent<Camera>();
		camera->tag = "MainCamera";
		newScene->AddGameObject(camera);

		AddScene(newScene);
		return newScene;
	}

	Scene* SceneManager::GetActiveScene()
	{
		return _activeScene;
	}

	Scene* SceneManager::GetSceneAt(int index)
	{
		return _sceneVector[index];
	}

	void SceneManager::AddScene(Scene* scene)
	{
		_sceneVector.push_back(scene);
	}

	bool SceneManager::SetActiveScene(Scene* scene)
	{
		Scene* sceneBackup = _activeScene;
		_activeScene = scene;

		// これは不要かな
		if (GameObject::FindGameObjectsWithTag("MainCamera").size() == 0)
		{
			Debug::Log(L"MainCameraがありません。");
		}

		return _activeScene != nullptr;
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		Scene* loadingScene = std_extension::Find<Scene*>(_sceneVector, [&](Scene* x) { return x->GetName() == sceneName; });
		SetActiveScene(loadingScene);
	}
}