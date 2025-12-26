#include "GroundChecker.h"

#include "GameObject.h"
#include "Player.h"

void GroundChecker::OnTriggerStay2D(GameObject* other)
{
	if (_triggerStayAction != nullptr)
	{
		_triggerStayAction();
	}
}

void GroundChecker::SetTriggerStayAction(std::function<void()> action)
{
	_triggerStayAction = action;
}
