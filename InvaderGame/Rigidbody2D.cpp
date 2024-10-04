#include "Rigidbody2D.h"

#include "GameEngine.h"

void Rigidbody2D::SetUseGravity(bool useGravity)
{
	_boxCollider2D = this->gameObject->GetComponent<BoxCollider2D>();
	_boxCollider2D->Getb2Body()->SetGravityScale(useGravity ? 1 : 0);
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
	//Debug::Log(L"%f", _boxCollider2D->Getb2Body()->GetGravityScale());

	if (_boxCollider2D != nullptr)
	{
		/*b2Vec2 vec2 = _boxCollider2D->Getb2Body()->GetPosition();
		Vector2 pos = Vector2(vec2.x, vec2.y);
		this->gameObject->transform->position = pos.ToVector3();*/

		b2Vec2 velocity = _boxCollider2D->Getb2Body()->GetLinearVelocity();
		Vector3 v = Vector3(velocity.x, velocity.y, 0);
		this->gameObject->transform->position = this->gameObject->transform->position + v;

		//_boxCollider2D->Getb2Body()->SetTransform()
	}
}