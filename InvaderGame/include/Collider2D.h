#pragma once

#include "framework.h"
#include "Component.h"

#include "Vector2.h"

class Collider2D : public Component
{
public:
	void SetOffset(Vector2 centerPos);
	b2Body* Getb2Body();
	b2PolygonShape* Getb2PolygonShape();

	bool IsAABB_Collided(Collider2D* collider);


protected:
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);

	b2BodyDef _bodyDef;
	b2Body* _body;
	b2PolygonShape dynamicBox;

	std::pair<Vector2, Vector2> GetAABB();
};