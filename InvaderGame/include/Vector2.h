#pragma once

#include "framework.h"
#include "Utility.h"

class Vector3;

class Vector2 : public SerializedClass
{
public:
	static const Vector2 up;

	static float Distance(const Vector2& vectorA, const Vector2& vectorB);
	static float Dot(const Vector2& vectorA, const Vector2& vectorB);
	static float Cross(const Vector2& vectorA, const Vector2& vectorB);
	
	float x, y;
	
	SERIALIZE3(SerializedClass,
		SERIALIZE_FIELD3(x),
		SERIALIZE_FIELD3(y)
	)

	Vector2();
	Vector2(float x, float y);

	float GetMagnitude();
	Vector2 Normalized();
	Vector3 ToVector3();
	Vector2 operator+ (const Vector2& vector) const;
	Vector2 operator- () const;
	Vector2 operator- (const Vector2& vector) const;
	Vector2 operator* (const float& value) const;
	Vector2 operator/ (const float& value) const;

	Vector2& operator+= (const Vector2& vector);
	Vector2& operator-= (const Vector2& vector);
	Vector2& operator*= (float value);
};