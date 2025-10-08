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

GameObject::GameObject()
{
	_transform = this->AddComponent<Transform>();
}

Transform* GameObject::GetTransform()
{
	return _transform;
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