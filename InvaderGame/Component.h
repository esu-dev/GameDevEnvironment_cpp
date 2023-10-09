#pragma once

#include "GameObject.h"

class Component
{
public:
	GameObject* gameObject;

	virtual void Update() = 0;
};

