#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector2.h"

class BoxCollider2D;

class Rigidbody2D : public Component
{
public:
	Vector2 position = Vector2(0, 0);
	void SetUseGravity(bool useGravity);
	void SetKinematic();
	void SetFreeze();
	void SetVelocity(Vector2 velocity);
	void Start() override;
	void Update() override;

private:
	BoxCollider2D* _boxCollider2D;
	b2FixtureDef fixtureDef;
};