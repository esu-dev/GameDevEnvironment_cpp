#pragma once

class Vector3;

class Vector2
{
public:
	float x, y;

	Vector2(float x, float y);

	Vector3 ToVector3();
	Vector2 operator+ (const Vector2& vector) const;
	Vector2 operator* (const float& value) const;
};