#include "Vector3.h"

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const Vector3 Vector3::operator+ (const Vector3& vector) const
{
	const Vector3 v = Vector3(this->x + vector.x, this->y + vector.y, this->z + vector.z);
	return v;
}

const Vector3 Vector3::operator* (const float& value) const
{
	const Vector3 v = Vector3(this->x * value, this->y * value, this->z * value);
	return v;
}

Vector3 Vector3::forward = Vector3(0, 0, 1);