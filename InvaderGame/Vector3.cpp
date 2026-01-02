#include "Vector3.h"

#include "GameEngine.h"


float Vector3::Dot(const Vector3& vectorA, const Vector3& vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

Vector3 Vector3::Cross(const Vector3& vectorA, const Vector3& vectorB)
{
	return Vector3(
		vectorA.y * vectorB.z - vectorA.z * vectorB.y,
		vectorA.z * vectorB.x - vectorA.x * vectorB.z,
		vectorA.x * vectorB.y - vectorA.y * vectorB.x);
}

float Vector3::Angle(Vector3 vectorA, Vector3 vectorB)
{
	float rad = DirectX::XMScalarACos(Dot(vectorA, vectorB) / (vectorA.GetMagnitude() * vectorB.GetMagnitude()));
	return DirectX::XMConvertToDegrees(rad);
}

float Vector3::SignedAngle(Vector3 from, Vector3 to, const Vector3& axis)
{
	Vector3 cross = Cross(from, to);
	float dot = Dot(cross, axis);
	if (dot < 0)
	{
		return Angle(from, to);
	}
	return 360.0f - Angle(from, to);
}

Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::AddX(float value)
{
	return Vector3(this->x + value, this->y, this->z);
}

Vector3 Vector3::AddY(float value)
{
	return Vector3(this->x, this->y + value, this->z);
}

Vector3 Vector3::AddZ(float value)
{
	return Vector3(this->x, this->y, this->z + value);
}

float Vector3::GetMagnitude()
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::GetNormalized()
{
	float square = x * x + y * y + z * z;
	if (square == 0)
	{
		return *this;
	}

	float norm = std::sqrt(square);
	return *this / norm;
}

Vector2 Vector3::ToVector2()
{
	return Vector2(this->x, this->y);
}

Vector3 Vector3::operator+ (const Vector3& vector) const
{
	const Vector3 v = Vector3(this->x + vector.x, this->y + vector.y, this->z + vector.z);
	return v;
}

void Vector3::operator+= (const Vector3& vector)
{
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;
}

Vector3 Vector3::operator-() const
{
	const Vector3 v = Vector3(-this->x, -this->y, -this->z);
	return v;
}

Vector3 Vector3::operator- (const Vector3& vector) const
{
	const Vector3 v = Vector3(this->x - vector.x, this->y - vector.y, this->z - vector.z);
	return v;
}

void Vector3::operator-= (const Vector3& vector)
{
	this->x -= vector.x;
	this->y -= vector.y;
	this->z -= vector.z;
}

Vector3 Vector3::operator* (const float& value) const
{
	const Vector3 v = Vector3(this->x * value, this->y * value, this->z * value);
	return v;
}

Vector3 Vector3::operator/ (const float& value) const
{
	const Vector3 v = Vector3(this->x / value, this->y / value, this->z / value);
	return v;
}

bool Vector3::operator==(const Vector3& vector) const
{
	return this->x == vector.x && this->y == vector.y && this->z == vector.z;
}

const Vector3 Vector3::zero = Vector3(0, 0, 0);
const Vector3 Vector3::one = Vector3(1, 1, 1);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);