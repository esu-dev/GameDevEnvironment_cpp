#include "Transform.h"

Transform::Transform()
{
	rotation = Quaternion::AngleAxis(30, Vector3::forward);
}

void Transform::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Transform::Update()
{
	D3D.SetRect(position.x, position.y, 100.0 * scale.x, 100.0 * scale.y, rotation);
}