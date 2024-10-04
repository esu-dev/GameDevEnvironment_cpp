#pragma once

class Vector2;

class Vector3
{
public:
	float x, y, z;

	Vector3(float x, float y, float z);

	Vector2 ToVector2();

	Vector3 operator+ (const Vector3& vector) const;
	Vector3 operator* (const float& value) const;
	static Vector3 forward;
};