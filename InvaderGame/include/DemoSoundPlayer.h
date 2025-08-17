#pragma once

#include "MonoBehaviour.h"

class DemoSoundPlayer : public MonoBehaviour
{
public:
	void Update() override;
	DemoSoundPlayer();

private:
	const float _interval = 3;
	const float _finishTime = 1.5f;
	float _timer = 0;
	bool _started = false;

	GameObject* _cube = new GameObject();
};