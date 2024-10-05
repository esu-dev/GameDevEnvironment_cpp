#include "SceneManager.h"

#include "GameEngine.h"

namespace SceneManagement
{
	// static•Ï”‚Ì’è‹`
	std::vector<Scene*> SceneManager::_sceneVector;
	Scene* SceneManager::_activeScene;

	Scene* SceneManager::CreateScene(std::string sceneName)
	{
		Scene* newScene = new Scene(sceneName);
		_sceneVector.push_back(newScene);

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

	bool SceneManager::SetActiveScene(Scene* scene)
	{
		_activeScene = scene;
		return _activeScene != nullptr;
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		Scene* loadingScene = std_extension::Find<Scene*>(_sceneVector, [&](Scene* x) { return x->GetName() == sceneName; });
		SetActiveScene(loadingScene);
		loadingScene->Start();
	}
}