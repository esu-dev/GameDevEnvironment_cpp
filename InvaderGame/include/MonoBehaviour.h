#pragma once

#include "Component.h"

class Collision2D;

class MonoBehaviour : public Component
{
public:
	virtual void OnCollisionEnter2D(Collision2D* collision) {}
};