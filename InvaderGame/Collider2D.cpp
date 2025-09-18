#include "Collider2D.h"

#include "GameEngine.h"

void Collider2D::SetOffset(Vector2 centerPos)
{
	_offset = centerPos;
}

b2Body* Collider2D::Getb2Body()
{
	return _body;
}

b2PolygonShape* Collider2D::Getb2PolygonShape()
{
	return &dynamicBox;
}

bool Collider2D::IsAABB_Collided(Collider2D* a, Collider2D* b)
{


	return true;
}
