#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"

using namespace DirectX;

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

	DirectX::XMMATRIX quaternionMatrix = this->ToXMMATRIX();

	DirectX::XMMATRIX vectorMatrix = DirectX::XMMATRIX(
		vector.x, 0, 0, 0,
		vector.y, 0, 0, 0,
		vector.z, 0, 0, 0,
		0, 0, 0, 0);

	DirectX::XMMATRIX result1 = quaternionMatrix * vectorMatrix;

	//// result1をクォータニオンに戻す
	Quaternion quaternion = Quaternion();
	quaternion.x = XMVectorGetX(result1.r[0]);
	quaternion.y = XMVectorGetX(result1.r[1]);
	quaternion.z = XMVectorGetX(result1.r[2]);
	quaternion.w = XMVectorGetX(result1.r[3]);

	// 4*4行列にする
	DirectX::XMMATRIX quaternionMatrix2 = quaternion.ToXMMATRIX();

	Quaternion invQuat = Quaternion::Inverse(*this);
	DirectX::XMMATRIX invQuatMatrix = DirectX::XMMATRIX(
		invQuat.x, 0, 0, 0,
		invQuat.y, 0, 0, 0,
		invQuat.z, 0, 0, 0,
		invQuat.w, 0, 0, 0);

	// 逆回転と掛け算する
	DirectX::XMMATRIX result2 = quaternionMatrix2 * invQuatMatrix;

	// ベクトルに直す
	Vector3 v = Vector3(XMVectorGetX(result2.r[0]), XMVectorGetX(result2.r[1]), XMVectorGetX(result2.r[2]));

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

DirectX::XMMATRIX Quaternion::ToXMMATRIX()
{
	DirectX::XMMATRIX matrix;

	matrix.r[0] = DirectX::XMVectorSet(this->w, -this->z, this->y, this->x);
	matrix.r[1] = DirectX::XMVectorSet(this->z, this->w, -this->x, this->y);
	matrix.r[2] = DirectX::XMVectorSet(-this->y, this->x, this->w, this->z);
	matrix.r[3] = DirectX::XMVectorSet(-this->x, -this->y, -this->z, this->w);

	return matrix;
}
