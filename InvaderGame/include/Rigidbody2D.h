#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector2.h"

#include "Collider2D.h"

class Rigidbody2D : public Component
{
public:
	Vector2 position = Vector2(0, 0);
	void SetUseGravity(bool useGravity);
	void SetKinematic();
	void SetFreeze();
	void SetDynamic();
	void SetVelocity(Vector2 velocity);
	void Start() override;
	void Update() override;

private:
	Vector2 _velocity = Vector2(0, 0);
	Collider2D* _collider2D;
	b2FixtureDef fixtureDef;
};