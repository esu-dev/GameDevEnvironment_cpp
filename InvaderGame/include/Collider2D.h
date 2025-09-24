#pragma once

#include "framework.h"
#include "Component.h"

#include "Vector2.h"

class BoxCollider2D;

class Collider2D : public Component
{
public:
	void Update() override;
	void SetOffset(Vector2 centerPos);
	b2Body* Getb2Body();
	b2PolygonShape* Getb2PolygonShape();

	bool IsAABB_Collided(Collider2D* collider);
	bool IsCollided(BoxCollider2D* collider);


protected:
	bool _showAABB = true;
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);

	b2BodyDef _bodyDef;
	b2Body* _body;
	b2PolygonShape dynamicBox;

	std::pair<Vector2, Vector2> GetAABB();
};