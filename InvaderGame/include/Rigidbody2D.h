#pragma once

#include <box2d.h>

#include "framework.h"
#include "Component.h"
#include "Vector2.h"
#include "Collider2D.h"
#include "Record.h"

class Rigidbody2D : public Component
{
public:
	bool IsKinematic = false;
	float GravityScale = 1;
	float mass = 1;
	//Vector2 velocity = Vector2(0, 0);
	Record<Vector2> velocity;

	NAME(Rigidbody2D)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(IsKinematic),
		SERIALIZE_FIELD3(GravityScale)
	)

	~Rigidbody2D() override {}

	void Start() override;
	void Update() override;

	void SetUseGravity(bool useGravity);
	void SetKinematic();
	void SetFreeze();
	void SetDynamic();
	void SetVelocity(Vector2 velocity);

	void ApplyGravity();
	void AddForce(Vector2 force);
	void AddImpulse(Vector2 impulse);
	

private:
	Collider2D* _collider2D;
	b2FixtureDef fixtureDef;
};