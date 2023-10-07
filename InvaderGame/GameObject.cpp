#include "GameObject.h"

void GameObject::Test()
{
	
}

void GameObject::Update()
{
	for (Component* component : m_componentVector)
	{
		component->Update();
	}
}