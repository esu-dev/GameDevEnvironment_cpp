#pragma once

#include "GameEngine.h"

class MonoBehaviour : public Component
{
public:
	virtual void OnCollisionEnter2D(Collision2D* collision) {}
};