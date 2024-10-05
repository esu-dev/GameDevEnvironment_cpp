#include "Matrix.h"

#include "Vector3.h"

Matrix Matrix::ConvertToMatrix(const Vector3& vector)
{
	Matrix matrix = Matrix();
	matrix.g_matrix = {
		{vector.x},
		{vector.y},
		{vector.z},
	};

	return matrix;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
	Matrix newMatrix = Matrix();

	int rowNum = this->g_matrix.size();
	int columnNum = matrix.g_matrix.front().size();

	for (int i = 0; i < rowNum; i++) // 4
	{
		std::vector<float> column;
		newMatrix.g_matrix.push_back(column);
		for (int j = 0; j < columnNum; j++) // 1
		{
			float v = 0;
			for (int s = 0; s < this->g_matrix.front().size(); s++) // 4
			{
				v += this->g_matrix[i][s] * matrix.g_matrix[s][j];
			}
			newMatrix.g_matrix[i].push_back(v);
		}
	}

	return newMatrix;
}