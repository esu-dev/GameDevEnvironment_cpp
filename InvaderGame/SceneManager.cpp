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
		_activeScene = scene;
		_activeScene->Start();
		return _activeScene != nullptr;
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		Scene* loadingScene = std_extension::Find<Scene*>(_sceneVector, [&](Scene* x) { return x->GetName() == sceneName; });
		SetActiveScene(loadingScene);
	}
}