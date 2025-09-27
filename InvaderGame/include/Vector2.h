#pragma once

class Vector3;

class Vector2
{
public:
	static float Distance(const Vector2& vectorA, const Vector2& vectorB);
	static float Dot(const Vector2& vectorA, const Vector2& vectorB);

	float x, y;

	Vector2();
	Vector2(float x, float y);

	Vector2 Normalized();
	Vector3 ToVector3();
	Vector2 operator+ (const Vector2& vector) const;
	Vector2 operator- () const;
	Vector2 operator- (const Vector2& vector) const;
	Vector2 operator* (const float& value) const;
	Vector2 operator/ (const float& value) const;

	Vector2& operator+= (const Vector2& vector);
};