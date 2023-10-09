#include "Quaternion.h"
#include "Vector3.h"

Quaternion& Quaternion::AngleAxis(float angle, Vector3 axis)
{
	Quaternion* quaternion = new Quaternion();

	float theta = angle * DirectX::XM_PI / 180;
	quaternion->x = axis.x * DirectX::XMScalarSin(theta / 2);
	quaternion->y = axis.y * DirectX::XMScalarSin(theta / 2);
	quaternion->z = axis.z * DirectX::XMScalarSin(theta / 2);
	quaternion->w = DirectX::XMScalarCos(theta / 2);

	return *quaternion;
}

Quaternion Quaternion::Inverse(Quaternion quaternion)
{
	Quaternion q = Quaternion();
	q.x = -quaternion.x;
	q.y = -quaternion.y;
	q.z = -quaternion.z;
	q.w = quaternion.w;

	return q;
}

Vector3 Quaternion::Mult(Vector3 vector)
{
	// クォータニオン行列×ベクトル

	Matrix quaternionMatrix = this->ToXMMATRIX();

	Matrix vectorMatrix = Matrix();
	vectorMatrix.g_matrix = {
		{ vector.x },
		{ vector.y },
		{ vector.z },
		{ 0 }
	};

	Debug::Log(L"start");
	Matrix result1 = quaternionMatrix * vectorMatrix;

	Debug::Log(L"result1: [%d][%d]", result1.g_matrix.size(), result1.g_matrix.front().size());

	// result1をクォータニオンに戻す
	Quaternion quaternion = Quaternion();
	quaternion.x = result1.g_matrix[0][0];
	quaternion.y = result1.g_matrix[1][0];
	quaternion.z = result1.g_matrix[2][0];
	quaternion.w = result1.g_matrix[3][0];

	// 4*4行列にする
	Matrix quaternionMatrix2 = quaternion.ToXMMATRIX();

	Quaternion inverseQuaternion = Quaternion::Inverse(*this);
	Matrix inverseQuaternionMatrix = Matrix();
	inverseQuaternionMatrix.g_matrix = {
		{ inverseQuaternion.x },
		{ inverseQuaternion.y },
		{ inverseQuaternion.z },
		{ inverseQuaternion.w }
	};

	// 逆回転と掛け算する
	Matrix result2 = quaternionMatrix2 * inverseQuaternionMatrix;

	// ベクトルに直す
	Vector3 v = Vector3(result2.g_matrix[0][0], result2.g_matrix[1][0], result2.g_matrix[2][0]); // resultでインデックスエラー

	return v;
}

Matrix Quaternion::ToXMMATRIX()
{
	Matrix matrix = Matrix();

	matrix.g_matrix = {
		{ this->w, -this->z, this->y, this->x },
		{ this->z, this->w, -this->x, this->y },
		{ -this->y, this->x, this->w, this->z },
		{ -this->x, -this->y, -this->z, this->w }
	};

	return matrix;
}

//Matrix::Matrix(int rowNum, int columnNum)
//{
//	for (int i = 0; i < rowNum; i++)
//	{
//		g_matrix.push_back(new std::vector<float>());
//		for (int j = 0; j < columnNum; j++)
//		{
//			g_matrix[i]->push_back(0);
//		}
//	}
//}

Matrix Matrix::operator*(const Matrix& matrix) const
{
	Matrix newMatrix = Matrix();

	int rowNum = this->g_matrix.size();
	int columnNum = matrix.g_matrix.front().size();

	Debug::Log(L"rowNum: %d", rowNum);
	Debug::Log(L"columnNum: %d", columnNum);
	Debug::Log(L"colimnNum_left: %d", this->g_matrix.front().size());

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

	Debug::Log(L"end*");

	return newMatrix;
}