#pragma once

#include "framework.h"


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

	private:
		static std::vector<Scene*> _sceneVector;
		static Scene* _activeScene;
	};
}