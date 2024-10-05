#pragma once

#include "framework.h"

class Vector3;

class Matrix
{
public:
	static Matrix ConvertToMatrix(const Vector3& vector);

	//Matrix(int rowNum, int columnNum);

	Matrix operator*(const Matrix& matrix) const;

	std::vector<std::vector<float>> g_matrix; // [çs][óÒ]
};