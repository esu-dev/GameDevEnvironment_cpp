#pragma once

#include "framework.h"
#include "Object.h"

#include "GameObject.h"

class Component : public Object
{
public:
	bool Started = false;
	GameObject* gameObject;

	virtual void OnEnable() {} // 本来はMonoBehaviourに定義
	virtual void OnDisable() {}
	virtual void Start() {}
	virtual void Update() {}

	template<typename T> T* GetComponent() // templateは分割コンパイルすると面倒
	{
		return this->gameObject->GetComponent<T>();
	}
};
