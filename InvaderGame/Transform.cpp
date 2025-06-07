#include "Transform.h"

Transform::Transform()
{
	rotation = Quaternion::AngleAxis(0, Vector3::forward);
}

Vector3 Transform::GetUp()
{
	return this->rotation * Vector3::up;
}

Vector3 Transform::GetForward()
{
	return this->rotation.Mult(Vector3::forward);
}

void Transform::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Transform::Update()
{
	//D3D.SetRect(position.x, position.y, scale.x, scale.y, rotation);
}