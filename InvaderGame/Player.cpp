#include "Player.h"

#include "Input.h"
#include "EngineTime.h"
#include "Transform.h"
#include "Rigidbody2d.h"

void Player::Update()
{
	// ˆÚ“®
	if (Input::GetKey('A'))
	{
		this->GetTransform()->position.Get().x -= _moveSpeed * EngineTime::GetDelataTime();
	}
	else if (Input::GetKey('D'))
	{
		this->GetTransform()->position.Get().x += _moveSpeed * EngineTime::GetDelataTime();
	}

	// ƒWƒƒƒ“ƒv
	if (Input::GetKey(VK_SPACE))
	{
		if (_canJump && _jumpCounter < _jumpTime)
		{
			_isGround = false;
			this->GetComponent<Rigidbody2D>()->AddForce(Vector2::up * _jumpPower);
		}
		
		_jumpCounter += EngineTime::GetDelataTime();
	}
	else if (!_isGround)
	{
		_canJump = false;
	}
}

void Player::OnCollisionEnter2D(GameObject* other)
{
	_isGround = true;
	_canJump = true;
	_jumpCounter = 0;
}