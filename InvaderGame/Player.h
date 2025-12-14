#pragma once

#include "EngineBehaviour.h"

class Player : public EngineBehaviour
{
public:
	NAME(Player)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_moveSpeed),
		SERIALIZE_FIELD3(_jumpTime),
		SERIALIZE_FIELD3(_jumpPower)
	)
	
	void Update() override;
	void OnCollisionEnter2D(GameObject* other) override;

private:
	float _moveSpeed = 1;
	float _jumpTime = 0.5f;
	float _jumpPower = 1;

	bool _isGround;
	bool _canJump;
	float _jumpCounter = 0;
};