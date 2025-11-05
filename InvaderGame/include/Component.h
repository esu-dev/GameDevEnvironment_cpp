#pragma once

#include "framework.h"
#include "Object.h"

#include "GameObject.h"

class Transform;

class Component : public Object
{
public:
	bool Started = false;
	GameObject* gameObject;

	SERIALIZE3(Object,
		SERIALIZE_FIELD3(gameObject)
	)

		virtual ~Component() { Debug::Log("Destructor is nor overrided. [%s]", GetName().c_str()); }
	virtual void OnEnable() {} // 本来はMonoBehaviourに定義
	virtual void OnDisable() {}
	virtual void Start() {}
	virtual void Update() {}

	Transform* GetTransform();

	template<typename T> T* GetComponent() // templateは分割コンパイルすると面倒
	{
		return this->gameObject->GetComponent<T>();
	}
};
