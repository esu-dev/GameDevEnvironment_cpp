#pragma once

#include "Component.h"

#include "Vector2.h"
#include "Record.h"

class VelocityController : public Component
{
public:
	NAME(VelocityController)

	Vector2 _initialVelocity;

	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_initialVelocity)
	)

	void Start() override;
};

