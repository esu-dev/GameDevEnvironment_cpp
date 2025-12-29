#pragma once

#include "framework.h"
#include "DirectX.h"

class Vector3;
class Matrix;

class Quaternion
{
public:
	float x, y, z, w;

	static Quaternion Identity();
	static Quaternion AngleAxis(float angle, Vector3 axis);
	static Quaternion Euler(const Vector3& euler);
	static Quaternion FromToRotation(Vector3 fromDirection, Vector3 toDirection);
	static Quaternion Inverse(Quaternion quaternion);

	Vector3 GetEulerAngles();
	Vector3 Mult(Vector3 vector);
	Quaternion operator-();
	Vector3 operator* (const Vector3& vector);

	Matrix ToXMMATRIX();
};
