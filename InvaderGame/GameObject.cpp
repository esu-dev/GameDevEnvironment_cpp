#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"
#include "Animator.h"

GameObject::GameObject()
	: transform{ AddComponent<Transform>() }
{

}

void GameObject::SetActive(bool isActive)
{
	_isActive = isActive;
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
	for (Component* component : m_componentVector)
	{
		component->Update();
	}
}