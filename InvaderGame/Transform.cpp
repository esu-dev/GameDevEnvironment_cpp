#include "Transform.h"

Transform::Transform()
{
	rotation = Quaternion::AngleAxis(0, Vector3::forward);
}

Transform::~Transform()
{
	// 子オブジェクトの親をなくす
	for (auto child : _childVector)
	{
		child->_parent = nullptr;
	}

	this->_parentProperty = nullptr;
}

void Transform::OnValidate()
{
	
}

void Transform::Update()
{
	if (_parent == nullptr)
	{
		return;
	}

	//_localPosition = position - parent->position;
	rotation = Quaternion::Euler(_parent->rotation.GetEulerAngles() + _localEulerRotation);
	position = _parent->position + _parent->rotation * _localPosition;
}

void Transform::EditorUpdate()
{
	if (_parent == nullptr)
	{
		return;
	}

	rotation = Quaternion::Euler(_parent->rotation.GetEulerAngles() + _localEulerRotation);
	position = _parent->position + _parent->rotation * _localPosition;
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
	position.Get().x = x;
	position.Get().y = y;

	if (_parentProperty == nullptr)
	{
		return;
	}
	_localPosition = position - _parent->position;
}

void Transform::SetLocalPosition(Vector3& vector)
{
	if (_parentProperty == nullptr)
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

void Transform::SetLocalRotation(Quaternion localRotation)
{
	_localEulerRotation = localRotation.GetEulerAngles();
}

Transform* Transform::GetParent()
{
	return _parent;
}

void Transform::SetParent(Transform* parent)
{
	this->_parentProperty = parent;
}

const std::vector<Transform*>& Transform::GetChildVector()
{
	return _childVector;
}

void Transform::AddChild(Transform* child)
{
	_childVector.push_back(child);
}

void Transform::RemoveChild(const Transform* child)
{
	std_extension::Remove(_childVector, (Transform*)child);
}