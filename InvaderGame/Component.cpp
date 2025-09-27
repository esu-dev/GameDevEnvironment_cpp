#include "Component.h"

Transform* Component::GetTransform()
{
	return _transform;
}

void Component::SetTransform(Transform* transform)
{
	_transform = transform;
}