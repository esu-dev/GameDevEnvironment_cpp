#pragma once

#include "EngineBehaviour.h"

class DemoTextFlash : public EngineBehaviour
{
public:
	NAME(DemoTextFlash)
	SERIALIZE3(EngineBehaviour,
		SERIALIZE_FIELD3(_visibleTime),
		SERIALIZE_FIELD3(_invisibleTime)
	)

	void Update() override;

private:
	float _visibleTime = 1;
	float _invisibleTime = 1;
};