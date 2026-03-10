#pragma once

#include "EngineFramework.h"


namespace SceneManagement
{
	class Scene;

	class SceneManager
	{
	public:
		static Scene* CreateScene(std::string sceneName);
		static Scene* GetActiveScene();
		static Scene* GetSceneAt(int index);
		static void AddScene(Scene* scene);
		static bool SetActiveScene(Scene* scene);
		static void LoadScene(std::string sceneName);
		static void LoadSceneAsset(const std::string& assetName);

	private:
		static std::vector<Scene*> _sceneVector;
		static Scene* _activeScene;
	};
}