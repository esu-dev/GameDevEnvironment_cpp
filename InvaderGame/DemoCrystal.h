#pragma once

#include "EngineBehaviour.h"

class DemoCrystal : public EngineBehaviour
{
public:
	NAME(DemoCrystal)

	void OnTriggerStay2D(GameObject* other) override;
};