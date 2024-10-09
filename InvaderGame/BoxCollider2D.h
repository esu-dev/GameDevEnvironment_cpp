#pragma once

#include "Collider2D.h"
#include "Vector2.h"

class BoxCollider2D : public Collider2D
{
public:
	BoxCollider2D();
	void SetOffset(Vector2 centerPos);
	void SetSize(Vector2 size);

	void OnDisable() override;
	void Start() override;
	void Update() override;

private:
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);

	void SynchronizeBodyWithGameObject();
};