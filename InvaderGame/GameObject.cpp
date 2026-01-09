#include "GameObject.h"

#include "GameSystem.h"
#include "SceneEditor.h"
#include "GameEngine.h"
#include "Transform.h"

GameObject::GameObject() : scene(nullptr)
{
	name = "GameObject";
}

std::vector<GameObject*> GameObject::FindGameObjectsWithTag(std::string tag)
{
	Scene* scene = SceneManagement::SceneManager::GetActiveScene();

	std::vector<GameObject*> gameObjectVector = std::vector<GameObject*>();

	if (scene == nullptr)
	{
		Debug::Log("Scene is null [SceneManager::GetActiveScene()]");
		return gameObjectVector;
	}

	for (GameObject* g : scene->GetGameObjectVector())
	{
		if (!g->ActiveSelf())
		{
			continue;
		}
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

void GameObject::RemoveComponent(const std::shared_ptr<Component>& component)
{
	GameSystem::GetInstance().AddDelayedExecution([&]() -> void {
		std_extension::Remove(_componentVector, component);
		//delete component;
	});
}

Transform* GameObject::GetTransform()
{
	return GetComponent<Transform>();
}

const std::vector<std::shared_ptr<Component>>& GameObject::GetComponentVector()
{
	return _componentVector;
}

void GameObject::SetActive(bool isActive)
{
	_isActive = isActive;

	if (_isActive)
	{
		for (std::shared_ptr<Component> component :  _componentVector)
		{
			component->OnEnable();
		}
	}
	else if (!_isActive)
	{
		for (std::shared_ptr<Component> component : _componentVector)
		{
			component->OnDisable();
		}
	}
}

bool GameObject::ActiveSelf()
{
	return _isActive;
}

void GameObject::Start()
{
	if (!_isActive)
	{
		return;
	}

	for (auto component : _componentVector)
	{
		if (!component->enabled)
		{
			continue;
		}

		if (!component.get()->Started)
		{
			component->Start();
			component.get()->Started = true;
		}
	}

	for (auto child : this->GetTransform()->GetChildVector())
	{
		child->gameObject->Start();
	}
}

void GameObject::Restart()
{
	if (!_isActive)
	{
		return;
	}

	for (auto component : _componentVector)
	{
		if (!component->enabled)
		{
			continue;
		}

		component->Restart();
		component.get()->Started = true;
	}

	for (auto child : this->GetTransform()->GetChildVector())
	{
		child->gameObject->Restart();
	}
}

void GameObject::Update()
{
	if (!_isActive)
	{
		return;
	}

	// コンポーネントのUpdateを呼び出す
	for (auto component : _componentVector)
	{
		if (!component->enabled)
		{
			continue;
		}

		component->Update();
	}

	// 子オブジェクトのUpdateを行う
	for (auto child : this->GetTransform()->GetChildVector())
	{
		child->gameObject->Update();
	}
}

void GameObject::EditorUpdate()
{
	if (!_isActive)
	{
		return;
	}

	// コンポーネントのUpdateを呼び出す
	for (auto component : _componentVector)
	{
		if (!component->enabled)
		{
			continue;
		}

		component->EditorUpdate();
	}

	// 子オブジェクトのUpdateを行う
	for (auto child : this->GetTransform()->GetChildVector())
	{
		child->gameObject->EditorUpdate();
	}
}