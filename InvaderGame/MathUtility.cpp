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

bool MathUtility::CalcLineCrossPoint(Vector2* outCrossPoint, const Vector2& linePointA, const Vector2& linePointB, const Vector2& linePointC, const Vector2& linePointD)
{
	Vector2 AB = linePointB - linePointA;
	Vector2 AC = linePointC - linePointA;
	Vector2 CA = linePointA - linePointC;
	Vector2 CD = linePointD - linePointC;

	float s = Vector2::Cross(AC, CD) / Vector2::Cross(AB, CD);
	Vector2 crossPoint = AB * s + linePointA;
	outCrossPoint->x = crossPoint.x;
	outCrossPoint->y = crossPoint.y;

	if (s >= 0 && s <= 1)
	{
		return true;
	}
	return false;
}