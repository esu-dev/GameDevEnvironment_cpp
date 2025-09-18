#pragma once

#include "Collider2D.h"

class CircleCollider2D : public Collider2D
{
public:
	void Update() override;

private:
	float _radius = 0.5f;
};