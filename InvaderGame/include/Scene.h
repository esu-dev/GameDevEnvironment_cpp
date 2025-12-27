#pragma once

#include "framework.h"

class GameObject;

namespace SceneManagement
{
	class Scene
	{
	public:
		Scene(std::string name);
		~Scene();
		std::string GetName();
		void SetName(std::string name);
		const std::vector<GameObject*>& GetGameObjectVector();
		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(GameObject* gameObject);
		void Start();
		void Update();

	private:
		std::string _name;
		std::vector<GameObject*> _gameObjectVector;
	};
}