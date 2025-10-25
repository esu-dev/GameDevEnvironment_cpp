#include "GameObject.h"

#include "GameEngine.h"


std::vector<GameObject*> GameObject::FindGameObjectsWithTag(std::string tag)
{
	Scene* scene = SceneManagement::SceneManager::GetActiveScene();

	std::vector<GameObject*> gameObjectVector = std::vector<GameObject*>();
	for (GameObject* g : scene->GetGameObjectVector())
	{
		if (g->tag == tag)
		{
			gameObjectVector.push_back(g);
		}
	}

	return gameObjectVector;
}

GameObject* GameObject::Create()
{
	GameObject* gameObject = new GameObject();
	gameObject->AddComponent<Transform>();
	return gameObject;
}

void GameObject::AddComponent(Component* component)
{
	std::shared_ptr<Component> sPtr_component = std::shared_ptr<Component>(component);
	sPtr_component.get()->gameObject = this;
	_componentVector.push_back(sPtr_component);
}

Transform* GameObject::GetTransform()
{
	return GetComponent<Transform>();
}

void GameObject::SetActive(bool isActive)
{
	_isActive = isActive;

	if (_isActive == false)
	{
		for (std::shared_ptr<Component> component : _componentVector)
		{
			component->OnDisable();
		}
	}
}

const std::vector<std::shared_ptr<Component>>& GameObject::GetComponentVector()
{
	return _componentVector;
}

void GameObject::Start()
{
	for (auto component : _componentVector)
	{
		if (!component.get()->Started)
		{
			component->Start();
			component.get()->Started = true;
		}
	}
}

void GameObject::Update()
{
	if (!_isActive)
	{
		return;
	}

	for (auto component : _componentVector)
	{
		component->Update();
	}
}