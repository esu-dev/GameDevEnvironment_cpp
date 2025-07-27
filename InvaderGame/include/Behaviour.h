#pragma once

#include "Component.h"
#include "GameObject.h"

class Transform;

class Behaviour : public Component
{
public:
	GameObject* get_gameObject()
	{
		return gameObject;
	}

	void set_gameObject(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}

	Transform* get_transform()
	{
		return gameObject->GetTransform();
	}
};