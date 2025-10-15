#pragma once

#include "MonoBehaviour.h"

class DemoPhysicsManager : public MonoBehaviour
{
public:
	void Update() override;
	DemoPhysicsManager();

private:
	const float _interval = 3;
	float _timer = 0;
	bool _started = false;

	GameObject* _cube = GameObject::Create();
	GameObject* _ground = GameObject::Create();
};