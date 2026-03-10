#pragma once

#include "EngineFramework.h"
#include "EngineBehaviour.h"

class GroundChecker : public EngineBehaviour
{
public:
	NAME(GroundChecker)

	void OnTriggerStay2D(GameObject* other) override;
	void SetTriggerStayAction(std::function<void()> action);

private:
	std::function<void()> _triggerStayAction;
};