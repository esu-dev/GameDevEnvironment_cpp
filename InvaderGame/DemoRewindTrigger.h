#pragma once

#include "EngineBehaviour.h"

class DemoRewinder;

class DemoRewindTrigger : public EngineBehaviour
{
public:
	NAME(DemoRewindTrigger)

	void OnEnable() override;
	void OnTriggerStay2D(GameObject* other) override;
	void Update() override;

	void Rewind();


private:
	float _timer = 0;
	std::vector<DemoRewinder*> _rewindObjectVec;
};