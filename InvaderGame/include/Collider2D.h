#pragma once

#include "framework.h"
#include "Component.h"

class Collider2D : public Component
{
public:
	b2Body* Getb2Body();
	b2PolygonShape* Getb2PolygonShape();


protected:
	b2BodyDef _bodyDef;
	b2Body* _body;
	b2PolygonShape dynamicBox;
};