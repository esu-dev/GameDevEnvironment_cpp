#pragma once

#include "Behaviour.h"

class Collision2D;

class MonoBehaviour : public Behaviour
{
public:
	virtual void OnCollisionEnter2D(Collision2D* collision) {}
};