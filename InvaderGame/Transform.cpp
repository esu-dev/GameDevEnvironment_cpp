#include "Transform.h"

Transform::Transform()
{
	rotation = Quaternion::AngleAxis(0, Vector3::forward);

	_record = 0;
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

	if (_parent == nullptr)
	{
		return;
	}
	_localPosition = position - _parent->position;
}

void Transform::SetLocalPosition(Vector3& vector)
{
	if (_parent == nullptr)
	{
		return;
	}

	_localPosition.x = vector.x;
	_localPosition.y = vector.y;
	
	position = _parent->position + _localPosition;
}

Vector3 Transform::GetLocalPosition()
{
	return _localPosition;
}

void Transform::SetParent(Transform* parent)
{
	this->_parent = parent;
}

void Transform::Update()
{
	_record = _record + 1;

	if (_parent == nullptr)
	{
		return;
	}

	//_localPosition = position - parent->position;
	position = _parent->position + _localPosition;
}