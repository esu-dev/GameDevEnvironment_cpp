#pragma once

#include "framework.h"
#include "Utility.h"
class Vector2;

class Vector3 : public SerializedClass
{
public:
	static float Dot(const Vector3& vectorA, const Vector3& vectorB);
	static Vector3 Cross(const Vector3& vectorA, const Vector3& vectorB);
	static float Angle(Vector3 vectorA, Vector3 vectorB);
	static float SignedAngle(Vector3 from, Vector3 to, const Vector3& axis);

	float x, y, z;

	SERIALIZE3(SerializedClass,
		SERIALIZE_FIELD3(x),
		SERIALIZE_FIELD3(y),
		SERIALIZE_FIELD3(z)
	)

	Vector3();
	Vector3(float x, float y, float z);

	Vector3 AddX(float value);
	Vector3 AddY(float value);
	Vector3 AddZ(float value);

	float GetMagnitude();
	Vector3 GetNormalized();
	Vector2 ToVector2();
	DirectX::XMVECTOR ToXMVECTOR();

	Vector3 operator+(const Vector3& vector) const;
	void operator+=(const Vector3& vector);
	Vector3 operator-() const;
	Vector3 operator-(const Vector3& vector) const;
	void operator-=(const Vector3& vector);
	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;
	bool operator==(const Vector3& vector) const;
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 up;
	static const Vector3 right;
	static const Vector3 forward;
};