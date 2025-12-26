#include "GimmickTrigger.h"

void GimmickTrigger::OnTriggerStay2D(GameObject* other)
{
	if (_triggerAction != nullptr)
	{
		_triggerAction();
	}
}

void GimmickTrigger::SetTriggerAction(std::function<void()> action)
{
	_triggerAction = action;
}