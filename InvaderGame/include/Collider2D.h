#pragma once

#include "EngineFramework.h"
#include "Component.h"

#include "Vector2.h"
#include "Collision2D.h"

class BoxCollider2D;

class Collider2D : public Component
{
public:
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_offset),
		SERIALIZE_FIELD3(_size)
	)

	virtual bool DetectCollision(Collision2D* outCollision, BoxCollider2D* collider) { return false; }
	virtual Vector2 GetClosestPoint(Vector2 point) { return Vector2(0, 0); };

	void Update() override;
	void SetOffset(Vector2 centerPos);

	bool IsAABB_Collided(Collider2D* collider);


protected:
	bool _showAABB = false;
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);

	std::pair<Vector2, Vector2> GetAABB();
	void GetOBBvertices(Vector2 outVertices[4]);
};