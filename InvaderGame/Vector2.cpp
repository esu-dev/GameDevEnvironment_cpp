#include "Vector2.h"

#include "GameEngine.h"

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2 Vector2::Normalized()
{
	float square = x * x + y * y;
	if (square == 0)
	{
		return *this;
	}

	float norm = std::sqrt(square);
	return *this / norm;
}

Vector3 Vector2::ToVector3()
{
	return Vector3(this->x, this->y, 0);
}

Vector2 Vector2::operator+ (const Vector2& vector) const
{
	const Vector2 v = Vector2(this->x + vector.x, this->y + vector.y);
	return v;
}

Vector2 Vector2::operator* (const float& value) const
{
	const Vector2 v = Vector2(this->x * value, this->y * value);
	return v;
}

Vector2 Vector2::operator/ (const float& value) const
{
	const Vector2 v = Vector2(this->x / value, this->y / value);
	return v;
}
