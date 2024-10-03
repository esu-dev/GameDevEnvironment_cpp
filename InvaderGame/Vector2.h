#pragma once

class Vector2
{
public:
	float x, y;

	Vector2(float x, float y);

	const Vector2 operator+ (const Vector2& vector) const;
	const Vector2 operator* (const float& value) const;
};