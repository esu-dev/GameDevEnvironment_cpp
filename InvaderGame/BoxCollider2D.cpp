#include "BoxCollider2D.h"

BoxCollider2D::BoxCollider2D()
{
	_bodyDef.type = b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	_body = GAMESYS.GetBox2DWorld()->CreateBody(&_bodyDef);

	dynamicBox.SetAsBox(10, 10);
}

void BoxCollider2D::SetOffset(Vector2 centerPos)
{
	_offset = centerPos;
}

void BoxCollider2D::SetSize(Vector2 size)
{
	_size = size;

	dynamicBox.SetAsBox(_size.x, _size.y);
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
	Vector2 colliderPos = this->gameObject->transform->position.ToVector2() + _offset;
	_body->SetTransform(b2Vec2{ colliderPos.x, colliderPos.y }, _body->GetAngle());
}