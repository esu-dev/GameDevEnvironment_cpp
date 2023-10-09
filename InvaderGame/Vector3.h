#pragma once

class Vector3
{
public:
	float x, y, z;

	Vector3(float x, float y, float z);

	const Vector3 operator+ (const Vector3& vector) const;
	const Vector3 operator* (const float& value) const;
	static Vector3 forward;
};