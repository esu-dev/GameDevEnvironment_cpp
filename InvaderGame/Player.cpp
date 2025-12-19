#include "Player.h"

#include "Input.h"
#include "EngineTime.h"
#include "Transform.h"
#include "Rigidbody2d.h"
#include "Animator.h"

void Player::Start()
{
	_idleState = new IdleState(this);
	_walkingState = new WalkingState(this);
	_currentState = _idleState;
}

void Player::Update()
{
	static bool isJumping = false;

	Rigidbody2D* rigidbody = this->GetComponent<Rigidbody2D>();
	Animator* animator = this->GetComponent<Animator>();

	// 移動
	if (Input::GetKey('A'))
	{
		rigidbody->AddForce(Vector2::left * _moveAcceleration);
		ChangeState(_walkingState);
	}
	else if (Input::GetKey('D'))
	{
		rigidbody->AddForce(Vector2::right * _moveAcceleration);
		ChangeState(_walkingState);
	}
	else
	{
		ChangeState(_idleState);
	}
	

	// ジャンプ
	if (_canJump && Input::GetKeyDown(VK_SPACE)) // getkeydownがなぜかずっとtrue
	{
		_canJump = false;
		isJumping = true;
		_jumpCounter = 0;
	}
	
	if (isJumping && _jumpCounter < _jumpTime)
	{
		rigidbody->AddForce(Vector2::up * _jumpPower);
		_jumpCounter += EngineTime::GetDelataTime();
	}
	else
	{
		isJumping = false;
	}



	// スピード調整
	if (abs(rigidbody->velocity.Get().x) > _maxSpeed)
	{
		rigidbody->velocity.Get().x = (rigidbody->velocity.Get().x > 0 ? 1 : -1) * _maxSpeed;
	}


	// State処理
	_currentState->Update();
}

void Player::OnCollisionEnter2D(GameObject* other) // たぶんずっと呼ばれてる
{
	_canJump = true;
}

void Player::ChangeState(State* state)
{
	if (_currentState == state)
	{
		return;
	}

	_currentState = state;
	_currentState->Enter();
}


Player::State::State(Player* player)
{
	this->player = player;
}

void Player::IdleState::Enter()
{
	Animator* animator = this->player->GetComponent<Animator>();
	animator->Play("IdleAnimation");
}

void Player::IdleState::Update()
{
	Rigidbody2D* rigidbody = this->player->GetComponent<Rigidbody2D>();
	if (abs(rigidbody->velocity.Get().x) > player->_stopAcceleration * EngineTime::GetFixedDeltaTime())
	{
		rigidbody->AddForce((Vector2::right * -rigidbody->velocity.Get().x).Normalized() * player->_stopAcceleration);
	}
}

void Player::WalkingState::Enter()
{
	Animator* animator = this->player->GetComponent<Animator>();
	animator->Play("WalkAnimation");
}