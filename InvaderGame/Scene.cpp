#include "Scene.h"

#include "GameEngine.h"

namespace SceneManagement
{
	Scene::Scene(std::string name) : _name(name) {}

	std::string Scene::GetName()
	{
		return _name;
	}

	const std::vector<GameObject*>& Scene::GetGameObjectVector()
	{
		return _gameObjectVector;
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		_gameObjectVector.push_back(gameObject);
		gameObject->scene = this;
		//gameObject->Start(); // Ç±ÇÍÇÕê≥ÇµÇ≠Ç»Ç¢ÇÊÇÀ
	}

	void Scene::RemoveGameObject(GameObject* gameObject)
	{
		//remove(_gameObjectVector.begin(), _gameObjectVector.end(), gameObject);
		std_extension::Remove(_gameObjectVector, gameObject);
	}

	void Scene::Start()
	{
		int num = _gameObjectVector.size();
		for (int i = 0; i < num; i++)
		{
			_gameObjectVector[i]->Start();
			num = _gameObjectVector.size();
		}
	}

	void Scene::Update()
	{
		int num = _gameObjectVector.size();
		for (int i = 0; i < num; i++)
		{
			_gameObjectVector[i]->Update();
			num = _gameObjectVector.size();
		}
	}
}