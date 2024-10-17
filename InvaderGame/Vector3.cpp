#include "Vector3.h"

#include "GameEngine.h"

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

Vector2 Vector3::ToVector2()
{
	return Vector2(this->x, this->y);
}

Vector3 Vector3::operator+ (const Vector3& vector) const
{
	const Vector3 v = Vector3(this->x + vector.x, this->y + vector.y, this->z + vector.z);
	return v;
}

Vector3 Vector3::operator* (const float& value) const
{
	const Vector3 v = Vector3(this->x * value, this->y * value, this->z * value);
	return v;
}

Vector3 Vector3::forward = Vector3(0, 0, 1);