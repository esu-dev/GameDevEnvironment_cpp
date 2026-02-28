#pragma once

#include "DirectX.h"
#include "Behaviour.h"

class Vector2;
class Vector3;
class Matrix;

class Camera : public Behaviour
{
public:
	static DirectX::XMFLOAT4 backgroundColor;

	static Vector3 WorldToViewportPoint(Vector3 worldPos);
	static Vector2 WorldToBox2DWorld(Vector3 worldPos);
	static Vector3 Box2DWorldToWorld(Vector2 worldPos);
	static Camera* GetMain();

	NAME(Camera)
	
	static const float Magnification;
	
	float GetSize();


private:
	float _size = 1.0f;
};