#pragma once

#include "EngineBehaviour.h"

class DemoRewinder : public EngineBehaviour
{
public:
	NAME(DemoRewinder)

	void Update() override;
	void Record();
	void Rewind();

private:
	bool _isRewinding = false;
	float _recordStartTime = 0.0f;
	float _rewindTime = 0.0f;
};