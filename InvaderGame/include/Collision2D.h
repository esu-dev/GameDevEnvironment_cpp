#pragma once

#include "framework.h"
#include "Vector2.h"

class Collider2D;

class Collision2D
{
public:
	struct CollisionData
	{
		float depth;
		Vector2 contact;
	};

	Collider2D* collider;
	Collider2D* otherCollider;

	std::vector<CollisionData*> collisionDataVector;
	Vector2 Normal;

	Collision2D(Collider2D* collider, Collider2D* otherCollider);
	Collider2D* GetCollider();
};