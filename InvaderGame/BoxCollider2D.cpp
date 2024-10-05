#include "BoxCollider2D.h"

BoxCollider2D::BoxCollider2D()
{
	_bodyDef.type = b2BodyType::b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	_body = GAMESYS.GetBox2DWorld()->CreateBody(&_bodyDef);

	dynamicBox.SetAsBox(4.0f, 4.0f);
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

	Debug::Log(L"%f, %f", v.x, v.y);
}

b2Body* BoxCollider2D::Getb2Body()
{
	return _body;
}

b2PolygonShape* BoxCollider2D::Getb2PolygonShape()
{
	return &dynamicBox;
}

void BoxCollider2D::Update()
{
	// b2Bodyの座標をオブジェクトと同期させる
	Vector2 colliderPos = Camera::WorldToBox2DWorld(this->gameObject->transform->position) + _offset;
	_body->SetTransform(b2Vec2{ colliderPos.x, colliderPos.y }, _body->GetAngle());

	//Debug::Log(L"%f, %f, %f, %f", _body->GetPosition().x, _body->GetPosition().y, this->gameObject->transform->position.x, this->gameObject->transform->position.x * Camera::Magnification / _body->GetPosition().x);
}