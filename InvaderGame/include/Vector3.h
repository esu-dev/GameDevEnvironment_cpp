#pragma once

#include "framework.h"
#include "Utility.h"
class Vector2;

class Vector3 : public SerializedClass
{
public:
	float x, y, z;

	SERIALIZE2(SerializedClass,
		SERIALIZE_FIELD2(x),
		SERIALIZE_FIELD2(y),
		SERIALIZE_FIELD2(z)
	)

	Vector3();
	Vector3(float x, float y, float z);

	Vector3 AddX(float value);
	Vector3 AddY(float value);
	Vector3 AddZ(float value);

	Vector3 GetNormalized();
	Vector2 ToVector2();

	Vector3 operator+(const Vector3& vector) const;
	Vector3 operator-() const;
	Vector3 operator-(const Vector3& vector) const;
	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;
	static Vector3 zero;
	static Vector3 up;
	static Vector3 right;
	static Vector3 forward;
};