#include "Vector2.h"

#include "GameEngine.h"

const Vector2 Vector2::zero = Vector2(0, 0);
const Vector2 Vector2::one = Vector2(1, 1);
const Vector2 Vector2::up = Vector2(0, 1);
const Vector2 Vector2::down = Vector2(0, -1);
const Vector2 Vector2::right = Vector2(1, 0);
const Vector2 Vector2::left = Vector2(-1, 0);

float Vector2::Distance(const Vector2& vectorA, const Vector2& vectorB)
{
	Vector2 v = vectorB - vectorA;
	return sqrt(v.x * v.x + v.y * v.y);
}

float Vector2::Dot(const Vector2& vectorA, const Vector2& vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

float Vector2::Cross(const Vector2& vectorA, const Vector2& vectorB)
{
	return vectorA.x * vectorB.y - vectorA.y * vectorB.x;
}

Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2::GetMagnitude()
{
	return sqrt(x * x + y * y);
}

float Vector2::Angle(Vector2 vectorA, Vector2 vectorB)
{
	float rad = DirectX::XMScalarACos(Dot(vectorA, vectorB) / (vectorA.GetMagnitude() * vectorB.GetMagnitude()));
	return DirectX::XMConvertToDegrees(rad);
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

Vector2 Vector2::operator- () const
{
	return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator- (const Vector2& vector) const
{
	const Vector2 v = Vector2(this->x - vector.x, this->y - vector.y);
	return v;
}

Vector2 Vector2::operator* (const float& value) const
{
	const Vector2 v = Vector2(this->x * value, this->y * value);
	return v;
}

Vector2 Vector2::operator* (const Vector2& value) const
{
	const Vector2 v = Vector2(this->x * value.x, this->y * value.y);
	return v;
}

Vector2 Vector2::operator/ (const float& value) const
{
	const Vector2 v = Vector2(this->x / value, this->y / value);
	return v;
}

Vector2& Vector2::operator+= (const Vector2& vector)
{
	*this = *this + vector;
	return *this;
}

Vector2& Vector2::operator-= (const Vector2& vector)
{
	*this = *this - vector;
	return *this;
}

Vector2& Vector2::operator*= (float value)
{
	*this = *this * value;
	return *this;
}
