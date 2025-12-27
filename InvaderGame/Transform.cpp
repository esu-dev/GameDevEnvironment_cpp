#include "Transform.h"

Transform::Transform()
{
	rotation = Quaternion::AngleAxis(0, Vector3::forward);
}

void Transform::OnValidate()
{
	// 親子関係の更新
	//if (_parent == nullptr)
	//{
	//	return;
	//}

	//// 親の子リストに自分が入っていなければ追加する
	//auto& parentChildVector = _parent->GetChildVector();
	//if (std::find(parentChildVector.begin(), parentChildVector.end(), this) == parentChildVector.end())
	//{
	//	_parent->AddChild(this);
	//}
	// 親の子リストに自分が入っているならば、削除する
	/*auto& parentChildVector = _preParent->GetChildVector();
	if (std::find(parentChildVector.begin(), parentChildVector.end(), this) != parentChildVector.end())
	{
		_preParent->RemoveChild(this);
	}*/
}

void Transform::Update()
{
	if (_parent == nullptr)
	{
		return;
	}

	//_localPosition = position - parent->position;
	position = _parent->position + _localPosition;
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

Transform* Transform::GetParent()
{
	return _parentProperty;
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