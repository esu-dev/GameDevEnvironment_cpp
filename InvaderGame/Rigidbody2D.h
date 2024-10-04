#pragma once

#include "framework.h"
#include "Component.h"

class BoxCollider2D;

class Rigidbody2D : public Component
{
public:
	void SetUseGravity(bool useGravity);
	void Start() override;
	void Update() override;

private:
	//Vector2 _velocity;
	BoxCollider2D* _boxCollider2D;
	b2FixtureDef fixtureDef;
};