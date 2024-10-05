#include "Scene.h"

#include "GameEngine.h"

namespace SceneManagement
{
	Scene::Scene(std::string name) : _name(name) {}

	std::string Scene::GetName()
	{
		return _name;
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		_gameObjectVector.push_back(gameObject);
	}

	void Scene::Start()
	{
		for (GameObject* gameObject : _gameObjectVector)
		{
			gameObject->Start();
		}
	}

	void Scene::Update()
	{
		for (GameObject* gameObject : _gameObjectVector)
		{
			gameObject->Update();
		}
	}
}