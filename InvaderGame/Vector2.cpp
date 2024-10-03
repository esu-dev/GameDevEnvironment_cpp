#include "Vector2.h"

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

const Vector2 Vector2::operator+ (const Vector2& vector) const
{
	const Vector2 v = Vector2(this->x + vector.x, this->y + vector.y);
	return v;
}

const Vector2 Vector2::operator* (const float& value) const
{
	const Vector2 v = Vector2(this->x * value, this->y * value);
	return v;
}
