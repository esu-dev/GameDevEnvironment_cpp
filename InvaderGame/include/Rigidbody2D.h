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
	bool IsKinematic;
	float mass = 1;
	//Vector2 velocity = Vector2(0, 0);
	Record<Vector2> velocity;

	void Start() override;
	void Update() override;

	void SetUseGravity(bool useGravity);
	void SetKinematic();
	void SetFreeze();
	void SetDynamic();
	void SetVelocity(Vector2 velocity);

	void AddImpulse(Vector2 impulse);
	

private:
	Collider2D* _collider2D;
	b2FixtureDef fixtureDef;
};