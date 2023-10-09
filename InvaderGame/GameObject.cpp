#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"

GameObject::GameObject()
	: transform{ AddComponent<Transform>() }
{

}

void GameObject::Update()
{
	for (Component* component : m_componentVector)
	{
		component->Update();
	}
}