#include "Scene.h"

#include "GameEngine.h"

namespace SceneManagement
{
	Scene::Scene(std::string name) : _name(name) {}

	Scene::~Scene()
	{
		for (GameObject* gameObject : _gameObjectVector)
		{
			delete gameObject;
		}
	}

	std::string Scene::GetName()
	{
		return _name;
	}

	void Scene::SetName(std::string name)
	{
		_name = name;
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
			// êeÇ™Ç¢ÇÈÇ»ÇÁUpdateÇµÇ»Ç¢
			if (_gameObjectVector[i]->GetTransform()->GetParent() != nullptr)
			{
				continue;
			}
			_gameObjectVector[i]->Update();
			num = _gameObjectVector.size();
		}
	}

	void Scene::EditorUpdate()
	{
		int num = _gameObjectVector.size();
		for (int i = 0; i < num; i++)
		{
			// êeÇ™Ç¢ÇÈÇ»ÇÁUpdateÇµÇ»Ç¢
			if (_gameObjectVector[i]->GetTransform()->GetParent() != nullptr)
			{
				continue;
			}
			_gameObjectVector[i]->EditorUpdate();
			num = _gameObjectVector.size();
		}
	}
}