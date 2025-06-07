#pragma once

#include "framework.h"
#include "DirectX.h"

class Vector3;
class Matrix;

class Quaternion
{
public:
	float x, y, z, w;

	static Quaternion& AngleAxis(float angle, Vector3 axis);
	static Quaternion Inverse(Quaternion quaternion);

	Vector3 Mult(Vector3 vector);
	Vector3 operator* (const Vector3& vector);

	Matrix ToXMMATRIX();
};
