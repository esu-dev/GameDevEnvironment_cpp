#include "Component.h"

#include "Transform.h"

Transform* Component::GetTransform()
{
	return this->gameObject->GetComponent<Transform>();
}
