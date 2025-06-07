#pragma once

class Vector2;

class Vector3
{
public:
	float x, y, z;

	Vector3(float x, float y, float z);

	Vector3 AddX(float value);
	Vector3 AddY(float value);
	Vector3 AddZ(float value);

	Vector2 ToVector2();

	Vector3 operator+ (const Vector3& vector) const;
	Vector3 operator* (const float& value) const;
	static Vector3 up;
	static Vector3 right;
	static Vector3 forward;
};