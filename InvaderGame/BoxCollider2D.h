#pragma once

#include "framework.h"
#include "GameEngine.h"

class BoxCollider2D : public Collider
{
public:
	void SetOffset(Vector2 centerPos);
	void SetSize(Vector2 size);

private:
	Vector2 _offset = Vector2(0, 0);
	Vector2 _size = Vector2(1, 1);
};