#pragma once

#include "framework.h"
#include "DirectX.h"

class Vector3;
class Matrix;

class Quaternion
{
public:
	float x, y, z, w;

	static Quaternion& Identity();
	static Quaternion& AngleAxis(float angle, Vector3 axis);
	static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);
	static Quaternion Inverse(Quaternion quaternion);

	Vector3 Mult(Vector3 vector);
	Quaternion operator-();
	Vector3 operator* (const Vector3& vector);

	Matrix ToXMMATRIX();
};
