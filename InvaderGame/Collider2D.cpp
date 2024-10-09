#include "Collider2D.h"

b2Body* Collider2D::Getb2Body()
{
	return _body;
}

b2PolygonShape* Collider2D::Getb2PolygonShape()
{
	return &dynamicBox;
}
