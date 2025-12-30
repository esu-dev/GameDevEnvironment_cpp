#pragma once

#include "EngineBehaviour.h"

class DemoClock : public EngineBehaviour
{
public:
	NAME(DemoClock)
	SERIALIZE3(EngineBehaviour,
		SERIALIZE_FIELD3(_minuteHandParent),
		SERIALIZE_FIELD3(_secondHandParent)
	)

	void Update() override;
	void SetRotationSpeed(float rotationSpeed);
	void Advance();
	void Rewind();

private:
	GameObject* _minuteHandParent;
	GameObject* _secondHandParent;

	bool _isAdvance = true;
	float _rotationSpeed;
};