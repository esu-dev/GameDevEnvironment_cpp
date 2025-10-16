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

	DESERIALIZE(
		DESERIALIZE_FIELD(gameObject)
	)

	virtual void OnEnable() {} // 本来はMonoBehaviourに定義
	virtual void OnDisable() {}
	virtual void Start() {}
	virtual void Update() {}

	Transform* GetTransform();
	void SetTransform(Transform* transform);

	template<typename T> T* GetComponent() // templateは分割コンパイルすると面倒
	{
		return this->gameObject->GetComponent<T>();
	}


private:
	Transform* _transform;
};
