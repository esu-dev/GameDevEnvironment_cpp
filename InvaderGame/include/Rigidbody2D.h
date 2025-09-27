#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector2.h"

#include "Collider2D.h"

class Rigidbody2D : public Component
{
public:
	float mass = 1;
	Vector2 velocity = Vector2(0, 0);

	void Start() override;
	void Update() override;

	void SetUseGravity(bool useGravity);
	void SetKinematic();
	void SetFreeze();
	void SetDynamic();
	void SetVelocity(Vector2 velocity);

	void AddImpulse(Vector2 impulse);
	

private:
	bool _isKinematic;
	
	Collider2D* _collider2D;
	b2FixtureDef fixtureDef;
};