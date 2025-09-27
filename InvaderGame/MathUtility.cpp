#include "MathUtility.h"

#include "GameEngine.h"

Vector2 MathUtility::CalcMinDisPointOnLine(Vector2 linePointA, Vector2 linePointB, Vector2 point)
{
	Vector2 AP = point - linePointA;
	Vector2 BP = point - linePointB;
	Vector2 AB = linePointB - linePointA;

	if (Vector2::Dot(AP, AB) < 0) return linePointA;
	else if (Vector2::Dot(BP, -AB) < 0) return linePointB;
	
	Vector2 direction = AB.Normalized();
	return linePointA + direction * Vector2::Dot(AP, direction);
}