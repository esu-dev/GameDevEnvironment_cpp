#pragma once

#include "EngineBehaviour.h"

class Player : public EngineBehaviour
{
public:
	NAME(Player)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_moveAcceleration),
		SERIALIZE_FIELD3(_maxSpeed),
		SERIALIZE_FIELD3(_stopAcceleration),
		SERIALIZE_FIELD3(_jumpTime),
		SERIALIZE_FIELD3(_jumpPower)
	)

	void Start() override;
	void Update() override;
	void OnCollisionEnter2D(GameObject* other) override;

private:
	class State
	{
	public:
		State(Player* player);
		virtual void Enter() {}
		virtual void Update() {}

	protected:
		Player* player;
	};

	class IdleState : public State
	{
	public:
		IdleState(Player* player) : State(player) {}
		void Enter() override;
		void Update() override;
	};

	class WalkingState : public State
	{
	public:
		WalkingState(Player* player) : State(player) {}
		void Enter() override;
	};

	float _moveAcceleration = 1;
	float _maxSpeed = 1;
	float _stopAcceleration = 0;
	float _jumpTime = 0.5f;
	float _jumpPower = 1;

	bool _canJump;
	float _jumpCounter = 0;

	State* _currentState;
	State* _idleState;
	State* _walkingState;

	void ChangeState(State* state);
};