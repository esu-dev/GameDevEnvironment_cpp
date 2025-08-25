#pragma once

#include "framework.h"
#include "Object.h"

class GameObject;

class Component : public Object
{
public:
	bool Started = false;
	GameObject* gameObject;

	virtual void OnEnable() {} // –{—ˆ‚ÍMonoBehaviour‚É’è‹`
	virtual void OnDisable() {}
	virtual void Start() {}
	virtual void Update() {}
};
