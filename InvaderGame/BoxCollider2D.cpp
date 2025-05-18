#include "BoxCollider2D.h"

#include "GameEngine.h"

BoxCollider2D::BoxCollider2D()
{
	_bodyDef.type = b2BodyType::b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	_body = Physics2D::GetBox2DWorld()->CreateBody(&_bodyDef);

	SetSize(4.0f, 4.0f);
}

void BoxCollider2D::SetOffset(Vector2 centerPos)
{
	_offset = centerPos;
}

void BoxCollider2D::SetSize(Vector2 size)
{
	_size = size;

	Vector2 v = Camera::WorldToBox2DWorld(size.ToVector3() * 0.5f);
	dynamicBox.SetAsBox(v.x, v.y);
}

void BoxCollider2D::SynchronizeBodyWithGameObject()
{
	Vector2 colliderPos = Camera::WorldToBox2DWorld(this->gameObject->GetTransform()->position) + _offset;
	_body->SetTransform(b2Vec2{ colliderPos.x, colliderPos.y }, _body->GetAngle());
}

void BoxCollider2D::OnDisable()
{
	GAMESYS.AddDelayedExecution([&]() { _body->SetEnabled(false); });
}

void BoxCollider2D::Start()
{
	SynchronizeBodyWithGameObject();
}

void BoxCollider2D::Update()
{
	// b2Bodyの座標をオブジェクトと同期させる
	SynchronizeBodyWithGameObject();

	//Debug::Log(L"%f, %f, %f, %f", _body->GetPosition().x, _body->GetPosition().y, this->gameObject->transform->position.x, this->gameObject->transform->position.x * Camera::Magnification / _body->GetPosition().x);
}