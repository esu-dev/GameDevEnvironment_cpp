#pragma once

class Vector2;

class MathUtility
{
public:
	static Vector2 CalcMinDisPointOnLine(Vector2 linePointA, Vector2 linePointB, Vector2 point);
	static bool CalcLineCrossPoint(Vector2* outCrossPoint, const Vector2& linePointA, const Vector2& linePointB, const Vector2& linePointC, const Vector2& linePointD);
};