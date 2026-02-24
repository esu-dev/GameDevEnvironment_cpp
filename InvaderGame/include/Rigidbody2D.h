#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector2.h"
#include "Collider2D.h"
#include "Record.h"

class Rigidbody2D : public Component
{
public:
	bool IsKinematic = false;
	bool IsTrigger;
	float GravityScale = 1;
	float mass = 1;
	//Vector2 velocity = Vector2(0, 0);
	Record<Vector2> velocity;

	NAME(Rigidbody2D)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(IsKinematic),
		SERIALIZE_FIELD3(IsTrigger),
		SERIALIZE_FIELD3(GravityScale),
		SERIALIZE_FIELD3(_isSleeping)
	)

	~Rigidbody2D() override {}

	void Start() override;
	void Update() override;

	bool IsSleeping();
	void SetKinematic();
	void SetDynamic();

	void ApplyGravity();
	void AddForce(Vector2 force);
	void AddImpulse(Vector2 impulse);
	

private:
	Record<bool> _isSleeping;
	float _sleepTimer = 0;
	Collider2D* _collider2D;
};