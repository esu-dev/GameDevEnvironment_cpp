#pragma once

#include "framework.h"
#include "GameEngine.h"

class BoxCollider2D : public Collider
{
public:
	BoxCollider2D();
	void SetOffset(Vector2 centerPos);
	void SetSize(Vector2 size);
	b2Body* Getb2Body();
	b2PolygonShape* Getb2PolygonShape();

	void Update() override;

private:
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);

	b2BodyDef _bodyDef;
	b2Body* _body;
	b2PolygonShape dynamicBox;
};