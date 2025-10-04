#include "TimeController.h"

#include "GameEngine.h"

void TimeController::Initialize()
{
	std::vector<InputSystem::KeySet> keySetVector;
	InputSystem::KeySet keySet_ctrl = InputSystem::KeySet();
	keySet_ctrl.isHold = true;
	keySet_ctrl.vkey = VK_CONTROL;

	InputSystem::KeySet keySet_space = InputSystem::KeySet();
	keySet_ctrl.isHold = false;
	keySet_ctrl.vkey = VK_SPACE;

	keySetVector.push_back(keySet_ctrl);
	keySetVector.push_back(keySet_space);

	InputSystem::AddKeyAction(keySetVector, []() -> void { Debug::Log(L"push"); });
}