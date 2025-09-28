#include "Rigidbody2D.h"

#include "GameEngine.h"

void Rigidbody2D::SetUseGravity(bool useGravity)
{
	_collider2D = this->gameObject->GetComponent<BoxCollider2D>();
	if (useGravity)
	{
		_collider2D->Getb2Body()->SetGravityScale(1);
	}
	else
	{
		_collider2D->Getb2Body()->SetGravityScale(0);
	}
}

void Rigidbody2D::SetKinematic()
{
	isKinematic = true;
	if (Physics2D::GetLibraryType() == Physics2D::LibraryType::Original)
	{
		return;
	}

	_collider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_collider2D->Getb2Body()->SetType(b2BodyType::b2_kinematicBody);
}

void Rigidbody2D::SetFreeze()
{
	_collider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_collider2D->Getb2Body()->SetType(b2BodyType::b2_staticBody);
}

void Rigidbody2D::SetDynamic()
{
	isKinematic = false;
	_collider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_collider2D->Getb2Body()->SetType(b2BodyType::b2_dynamicBody);
}

void Rigidbody2D::SetVelocity(Vector2 velocity)
{
	Vector2 box2DVelocity = Camera::WorldToBox2DWorld(velocity.ToVector3());
	b2Vec2 v = b2Vec2{ box2DVelocity.x, box2DVelocity.y };
	_collider2D->Getb2Body()->SetLinearVelocity(v);
}

void Rigidbody2D::Start()
{
	_collider2D = this->gameObject->GetComponent<Collider2D>();

	if (Physics2D::GetLibraryType() == Physics2D::LibraryType::Box2D)
	{
		fixtureDef.shape = _collider2D->Getb2PolygonShape();
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		_collider2D->Getb2Body()->CreateFixture(&fixtureDef);
	}
}

void Rigidbody2D::Update()
{
	if (Physics2D::GetLibraryType() == Physics2D::LibraryType::Original)
	{
		// 位置の更新
		this->gameObject->GetTransform()->position = this->gameObject->GetTransform()->position + velocity.ToVector3() * Time::FixedDeltaTime;

		return;
	}

	// 位置の更新
	if (_collider2D != nullptr)
	{
		b2Vec2 velocity = _collider2D->Getb2Body()->GetLinearVelocity();
		Vector2 v = Vector2(velocity.x, velocity.y);
		this->gameObject->GetTransform()->position = this->gameObject->GetTransform()->position + Camera::Box2DWorldToWorld(v) * Time::FixedDeltaTime;

		//Debug::Log(L"pos: %f, delta: %f", this->gameObject->GetTransform()->position.y, Time::GetDelataTime());
	}
}

void Rigidbody2D::AddImpulse(Vector2 impulse)
{
	if (isKinematic) return;

	velocity += impulse / mass;
}