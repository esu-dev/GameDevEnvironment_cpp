#include "TimeController.h"

#include "GameEngine.h"
#include "RecordManager.h"

void TimeController::Initialize()
{
	static int backNum = 0;

	InputSystem::KeySet keySet_ctrl = InputSystem::KeySet();
	keySet_ctrl.isHold = true;
	keySet_ctrl.vkey = VK_CONTROL;

	{
		InputSystem::KeySet keySet_space = InputSystem::KeySet();
		keySet_space.isHold = false;
		keySet_space.vkey = VK_SPACE;

		std::vector<InputSystem::KeySet> keySetVector;
		keySetVector.push_back(keySet_ctrl);
		keySetVector.push_back(keySet_space);

		InputSystem::AddKeyAction(keySetVector, []() -> void {
			Debug::Log(L"Enter Editor");
			backNum = 0;
			EngineTime::TimeScale = 0;
		});
	}

	{
		InputSystem::KeySet keySet_space = InputSystem::KeySet();
		keySet_space.isHold = false;
		keySet_space.vkey = VK_SPACE;

		std::vector<InputSystem::KeySet> keySetVector;
		keySetVector.push_back(keySet_space);

		InputSystem::AddKeyAction(keySetVector, []() -> void {
			Debug::Log(L"Exit Editor");
			float time = EngineTime::_totalTime - RecordBase::RECORD_INTERVAL * (backNum - 1);
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Decide(time);
			}
			EngineTime::_totalTime = (int)(time / RecordBase::RECORD_INTERVAL);
			EngineTime::TimeScale = 1;
		});
	}

	{
		InputSystem::KeySet keySet = InputSystem::KeySet();
		keySet.isHold = false;
		keySet.vkey = VK_LEFT;

		std::vector<InputSystem::KeySet> keySetVector;
		keySetVector.push_back(keySet);

		InputSystem::AddKeyAction(keySetVector, []() -> void {
			Debug::Log(L"--");
			backNum++;
			float time = EngineTime::GetTotalTime() - RecordBase::RECORD_INTERVAL * (backNum - 1);
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Select(time);
			}
		});
	}

	{
		InputSystem::KeySet keySet = InputSystem::KeySet();
		keySet.isHold = false;
		keySet.vkey = VK_RIGHT;

		std::vector<InputSystem::KeySet> keySetVector;
		keySetVector.push_back(keySet);

		InputSystem::AddKeyAction(keySetVector, []() -> void {
			Debug::Log(L"++");
			backNum = backNum <= 1 ? 1 : backNum - 1;
			float time = EngineTime::GetTotalTime() - RecordBase::RECORD_INTERVAL * (backNum - 1);
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Select(time);
			}
			});
	}
}