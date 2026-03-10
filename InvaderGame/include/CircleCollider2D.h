#pragma once

#include "Collider2D.h"

class CircleCollider2D : public Collider2D
{
public:
	void Update() override;
	bool DetectCollision(Collision2D* outCollision, BoxCollider2D* collider) override;
	//Vector2 GetMinPoint(Vector2 point) override;

private:
	float _radius = 0.5f;
};