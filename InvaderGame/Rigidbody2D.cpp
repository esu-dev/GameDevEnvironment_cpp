#include "Rigidbody2D.h"

#include "GameEngine.h"

void Rigidbody2D::SetUseGravity(bool useGravity)
{
	_boxCollider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_boxCollider2D->Getb2Body()->SetGravityScale(useGravity ? 1.0f : 0.0f);
}

void Rigidbody2D::SetKinematic()
{
	_boxCollider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_boxCollider2D->Getb2Body()->SetType(b2BodyType::b2_kinematicBody);
}

void Rigidbody2D::SetFreeze()
{
	_boxCollider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_boxCollider2D->Getb2Body()->SetType(b2BodyType::b2_staticBody);
}

void Rigidbody2D::SetVelocity(Vector2 velocity)
{
	Vector2 box2DVelocity = Camera::WorldToBox2DWorld(velocity.ToVector3());
	b2Vec2 v = b2Vec2{ box2DVelocity.x, box2DVelocity.y };
	_boxCollider2D->Getb2Body()->SetLinearVelocity(v);
}

void Rigidbody2D::Start()
{
	_boxCollider2D = this->gameObject->GetComponent<BoxCollider2D>();

	fixtureDef.shape = _boxCollider2D->Getb2PolygonShape();
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_boxCollider2D->Getb2Body()->CreateFixture(&fixtureDef);
}

void Rigidbody2D::Update()
{
	if (_boxCollider2D != nullptr)
	{
		b2Vec2 velocity = _boxCollider2D->Getb2Body()->GetLinearVelocity();
		Vector2 v = Vector2(velocity.x, velocity.y);
		this->gameObject->GetTransform()->position = this->gameObject->GetTransform()->position + Camera::Box2DWorldToWorld(v) * Time::GetDelataTime();
	}
}