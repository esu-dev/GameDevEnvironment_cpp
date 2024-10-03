#include "BoxCollider2D.h"

void BoxCollider2D::SetOffset(Vector2 centerPos)
{
	_offset = centerPos;
}

void BoxCollider2D::SetSize(Vector2 size)
{
	_size = size;
}