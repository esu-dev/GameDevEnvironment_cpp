#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"

Quaternion Quaternion::Identity()
{
	return AngleAxis(0, Vector3::forward);
}

Quaternion Quaternion::AngleAxis(float angle, Vector3 axis)
{
	Quaternion quaternion;

	float theta = angle * DirectX::XM_PI / 180;
	quaternion.x = axis.x * DirectX::XMScalarSin(theta / 2);
	quaternion.y = axis.y * DirectX::XMScalarSin(theta / 2);
	quaternion.z = axis.z * DirectX::XMScalarSin(theta / 2);
	quaternion.w = DirectX::XMScalarCos(theta / 2);

	return quaternion;
}

// 一旦2D限定
Quaternion Quaternion::Euler(const Vector3& euler)
{
	return AngleAxis(euler.z, Vector3::forward);
}

// 一旦2D限定
Quaternion Quaternion::FromToRotation(Vector3 fromDirection, Vector3 toDirection)
{
	return AngleAxis(Vector2::Angle(fromDirection.ToVector2(), toDirection.ToVector2()), Vector3::forward);
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

// 一旦2D限定
Vector3 Quaternion::GetEulerAngles()
{
	Vector3 v = *this * Vector3::up;
	float z = Vector3::SignedAngle(v, Vector3::up, Vector3::forward);
	return Vector3(0, 0, z);
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

	Matrix result1 = quaternionMatrix * vectorMatrix;

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

Quaternion Quaternion::operator-()
{
	return Inverse(*this);
}

Vector3 Quaternion::operator*(const Vector3& vector)
{
	return this->Mult(vector);
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
