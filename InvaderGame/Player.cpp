#include "Player.h"

#include "Input.h"
#include "EngineTime.h"
#include "Transform.h"
#include "Rigidbody2d.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "GroundChecker.h"
#include "DemoRewindTrigger.h"

void Player::Start()
{
	_idleState = new IdleState(this);
	_walkingState = new WalkingState(this);
	_jumpingState = new JumpingState(this);
	_currentState = _idleState;
}

void Player::Update()
{
	Rigidbody2D* rigidbody = this->GetComponent<Rigidbody2D>();
	Animator* animator = this->GetComponent<Animator>();

	// 移動
	bool _isMoving = false;
	bool _isMovingLeft = false;
	if (Input::GetKey(VK_LEFT))
	{
		_isMoving = true;
		_isMovingLeft = true;
		rigidbody->AddForce(Vector2::left * _moveAcceleration);
	}
	else if (Input::GetKey(VK_RIGHT))
	{
		_isMoving = true;
		_isMovingLeft = false;
		rigidbody->AddForce(Vector2::right * _moveAcceleration);
	}
	else if (_currentState != _jumpingState)
	{
		ChangeState(_idleState);
	}

	// 移動中
	if (_isMoving)
	{
		if (_currentState != _jumpingState)
		{
			ChangeState(_walkingState);
		}

		// 左右に応じてスプライトを反転する
		SpriteRenderer* spriteRenderer = this->GetComponent<SpriteRenderer>();
		if (spriteRenderer == nullptr) return;
		spriteRenderer->SetFlip(_isMovingLeft);
	}
	// 移動入力していない処理
	else
	{
		if (abs(rigidbody->velocity.Get().x) > _stopAcceleration * EngineTime::GetFixedDeltaTime())
		{
			rigidbody->AddForce((Vector2::right * -rigidbody->velocity.Get().x).Normalized() * _stopAcceleration);
		}
	}
	

	// ジャンプ
	if (_canJump && Input::GetKeyDown(VK_SPACE)) // getkeydownがなぜかずっとtrue
	{
		_canJump = false;
		rigidbody->SetVelocity(Vector2(0, 0));
		rigidbody->AddImpulse(Vector2::up * _jumpPower);
		ChangeState(_jumpingState);
	}


	// スピード調整
	if (abs(rigidbody->velocity.Get().x) > _maxSpeed)
	{
		rigidbody->velocity.Get().x = (rigidbody->velocity.Get().x > 0 ? 1 : -1) * _maxSpeed;
	}


	// 記録開始
	if (Input::GetKeyDown('Z'))
	{
		_recordCircle->GetComponent<DemoRewindTrigger>()->RecordTriggerOn();
	}

	// 巻き戻し開始
	if (Input::GetKeyDown('X'))
	{
		_recordCircle->GetComponent<DemoRewindTrigger>()->Rewind();
		_rewindCircle->GetComponent<Animator>()->Play("RewindAnimation");
	}


	// State処理
	_currentState->Update();
}

void Player::ChangeState(State* state)
{
	if (_currentState == state)
	{
		return;
	}

	// call exit on current before switching
	if (_currentState)
	{
		_currentState->Exit();
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
	
}

void Player::WalkingState::Enter()
{
	Animator* animator = this->player->GetComponent<Animator>();
	animator->Play("WalkAnimation");
}

void Player::WalkingState::Update()
{
	
}

void Player::JumpingState::Enter()
{
	Animator* animator = this->player->GetComponent<Animator>();
	animator->Play("JumpAnimation");

	for (Transform* transform : this->player->GetTransform()->GetChildVector())
	{
		_groundChecker = transform->gameObject->GetComponent<GroundChecker>();
		if (_groundChecker != nullptr)
		{
			break;
		}
	}
}

void Player::JumpingState::Update()
{
	if (_groundChecker == nullptr)
	{
		return;
	}

	if (this->player->GetComponent<Rigidbody2D>()->velocity.Get().y <= 0)
	{
		_groundChecker->SetTriggerStayAction([this]() {
			this->player->_canJump = true;

			// 着地したらIdleStateに遷移
			this->player->ChangeState(this->player->_idleState);
			});
	}
	else
	{
		_groundChecker->SetTriggerStayAction(nullptr);
	}
}

void Player::JumpingState::Exit()
{
	if (_groundChecker != nullptr)
	{
		_groundChecker->SetTriggerStayAction(nullptr);
	}
}