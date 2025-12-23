#pragma once

#include "Behaviour.h"

class GameObject;

class EngineBehaviour : public Behaviour
{
public:
	virtual void OnCollisionEnter2D(GameObject* other) {}
	virtual void OnTriggerStay2D(GameObject* other) {}
};