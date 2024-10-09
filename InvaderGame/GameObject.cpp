#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"
#include "Animator.h"

GameObject::GameObject()
	: _transform{ AddComponent<Transform>() }
{

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
		for (Component* component : m_componentVector)
		{
			component->OnDisable();
		}
	}
}

void GameObject::Start()
{
	for (Component* component : m_componentVector)
	{
		component->Start();
	}
}

void GameObject::Update()
{
	if (!_isActive)
	{
		return;
	}

	for (Component* component : m_componentVector)
	{
		component->Update();
	}
}