#pragma once

class Vector2;
class Vector3;
class Matrix;

class Camera
{
public:
	static const float Magnification;
	static Vector3 WorldToViewportPoint(Vector3 worldPos);
	static Vector2 WorldToBox2DWorld(Vector3 worldPos);
	static Vector3 Box2DWorldToWorld(Vector2 worldPos);
};