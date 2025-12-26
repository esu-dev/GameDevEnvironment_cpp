#pragma once

#include "EngineBehaviour.h"

class GimmickTrigger : public EngineBehaviour
{
public:
	NAME(GimmickTrigger)

	void OnTriggerStay2D(GameObject* other) override;
	void SetTriggerAction(std::function<void()> action);

private:
	std::function<void()> _triggerAction;
};