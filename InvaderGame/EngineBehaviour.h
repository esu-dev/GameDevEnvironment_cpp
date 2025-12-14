#pragma once

#include "Component.h"

class GameObject;

class EngineBehaviour : public Component
{
public:
	virtual void OnCollisionEnter2D(GameObject* other) {}
};