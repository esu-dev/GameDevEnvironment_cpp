#include "Rigidbody2D.h"

#include "GameEngine.h"

bool Rigidbody2D::IsSleeping()
{
	return _isSleeping;
}

void Rigidbody2D::SetKinematic()
{
	IsKinematic = true;
}

void Rigidbody2D::SetDynamic()
{
	IsKinematic = false;
}

void Rigidbody2D::Start()
{
	_collider2D = this->gameObject->GetComponent<Collider2D>();
}

void Rigidbody2D::Update()
{
	// 自作物理エンジン
	/*if (velocity.Get().GetMagnitude() < 1)
	{
		_sleepTimer += EngineTime::GetDeltaTime();

		if (_sleepTimer >= 3)
		{
			_isSleeping = true;
		}
	}
	else
	{
		_isSleeping = false;
		_sleepTimer = 0;
	}*/

	if (velocity.Get().GetMagnitude() == 0)
	{
		return;
	}

	/*if (_isSleeping)
	{
		return;
	}*/
		
	// 位置の更新
	this->gameObject->GetTransform()->position = this->gameObject->GetTransform()->position.Get() + ((Vector2)velocity).ToVector3() * EngineTime::GetFixedDeltaTime();
}

void Rigidbody2D::ApplyGravity()
{
	if (IsKinematic)
	{
		return;
	}

	// スリープで落下しないようになると困る
	/*if (_isSleeping)
	{
		return;
	}*/

	velocity += Vector2(0, -Physics2D::GRAVITATIONAL_ACCELERATION) * GravityScale * EngineTime::GetFixedDeltaTime();
}

void Rigidbody2D::AddForce(Vector2 force)
{
	velocity += force / mass * EngineTime::GetFixedDeltaTime();
}

void Rigidbody2D::AddImpulse(Vector2 impulse)
{
	if (IsKinematic) return;
	//Debug::Log("(%g, %f)", impulse.x, impulse.y);
	velocity += impulse / mass;
}