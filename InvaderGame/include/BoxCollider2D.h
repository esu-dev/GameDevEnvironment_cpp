#pragma once

#include "Collider2D.h"
#include "Vector2.h"

class BoxCollider2D : public Collider2D
{
public:
	NAME(BoxCollider2D)

	BoxCollider2D();
	~BoxCollider2D() override;
	void SetSize(Vector2 size);

	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;
	Vector2 GetClosestPoint(Vector2 point) override;
	bool DetectCollision(Collision2D* outCollision, BoxCollider2D* collider) override;

	Vector2 GetClosestPointByVertex(const Vector2 vertices[4], int targetIndex);
	bool IsPointIn(Vector2& point);


private:
	void SynchronizeBodyWithGameObject();
};
