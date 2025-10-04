#include "TimeController.h"

#include "GameEngine.h"
#include "RecordManager.h"

void TimeController::Initialize()
{
	InputSystem::KeySet keySet_ctrl = InputSystem::KeySet();
	keySet_ctrl.isHold = true;
	keySet_ctrl.vkey = VK_CONTROL;

	InputSystem::KeySet keySet_space = InputSystem::KeySet();
	keySet_space.isHold = false;
	keySet_space.vkey = VK_SPACE;

	std::vector<InputSystem::KeySet> keySetVector;
	keySetVector.push_back(keySet_ctrl);
	keySetVector.push_back(keySet_space);

	InputSystem::AddKeyAction(keySetVector, []() -> void {
			Debug::Log(L"push");
			//Time::TimeScale = 0;
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Back();
			}
		});
}