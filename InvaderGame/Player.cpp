#include "Player.h"

#include "Input.h"
#include "EngineTime.h"
#include "Transform.h"
#include "Rigidbody2d.h"

void Player::Update()
{
	Rigidbody2D* rigidbody = this->GetComponent<Rigidbody2D>();

	// ˆÚ“®
	if (Input::GetKey('A'))
	{
		rigidbody->AddForce(Vector2::left * _moveSpeed);
	}
	else if (Input::GetKey('D'))
	{
		rigidbody->AddForce(Vector2::right * _moveSpeed);

	}

	// ƒWƒƒƒ“ƒv
	if (Input::GetKey(VK_SPACE))
	{
		if (_canJump && _jumpCounter < _jumpTime)
		{
			_isGround = false;
			rigidbody->AddForce(Vector2::up * _jumpPower);
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