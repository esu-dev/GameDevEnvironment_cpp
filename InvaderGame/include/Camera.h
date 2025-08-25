#pragma once

#include "Behaviour.h"

class Vector2;
class Vector3;
class Matrix;

class Camera : public Behaviour
{
public:
	static Vector3 WorldToViewportPoint(Vector3 worldPos);
	static Vector2 WorldToBox2DWorld(Vector3 worldPos);
	static Vector3 Box2DWorldToWorld(Vector2 worldPos);
	static Camera* get_main();
	
	static const float Magnification;
};