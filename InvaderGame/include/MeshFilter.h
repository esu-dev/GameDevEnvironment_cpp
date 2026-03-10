#pragma once

#include "Component.h"

class Mesh;

class MeshFilter : public Component
{
private:
	Mesh* _mesh;
};