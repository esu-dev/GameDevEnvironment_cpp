#include "TimeController.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "GameSystem.h"
#include "GameEngine.h"
#include "RecordManager.h"


void TimeController::Initialize()
{
	static int backNum = 0;

	InputSystem::KeySet keySet_ctrl = InputSystem::KeySet();
	keySet_ctrl.isHold = true;
	keySet_ctrl.vkey = VK_CONTROL;

	InputSystem::AddKeyAction({ InputSystem::KeySet('T') }, []() -> void {
		_isEditorON = !_isEditorON;
		if (_isEditorON)
		{
			Debug::Log(L"Enter Editor");
			backNum = 0;
			EngineTime::TimeScale = 0;
		}
		else
		{
			Debug::Log(L"Exit Editor");
			//float time = EngineTime::TotalTime - RecordBase::RECORD_INTERVAL * (backNum - 1);
			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Decide(_time);
			}
			//EngineTime::TotalTime = (int)(time / RecordBase::RECORD_INTERVAL);
			EngineTime::TotalTime = _time;
			EngineTime::TimeScale = 1;
		}
	});

	InputSystem::AddKeyAction({ InputSystem::KeySet(VK_LEFT) }, []() -> void {
		Debug::Log(L"--");
		backNum++;
		float time = EngineTime::GetTotalTime() - RecordBase::RECORD_INTERVAL * (backNum - 1);
		for (RecordBase* record : RecordManager::RecordVector)
		{
			record->Select(time);
		}
	});

	InputSystem::AddKeyAction({ InputSystem::KeySet(VK_RIGHT) }, []() -> void {
		Debug::Log(L"++");
		backNum = backNum <= 1 ? 1 : backNum - 1;
		float time = EngineTime::GetTotalTime() - RecordBase::RECORD_INTERVAL * (backNum - 1);
		for (RecordBase* record : RecordManager::RecordVector)
		{
			record->Select(time);
		}
	});
}

void TimeController::Update()
{
	// 
	static int width = 300;
	static int height = 200;

	static int time = 0;

	ImGui::SetNextWindowPos(ImVec2(GameSystem::WINDOW_WIDTH - width - 10, GameSystem::WINDOW_HEIGHT - height - 10));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	if (ImGui::Begin("Time Controller"))
	{
		int totalTime_int = EngineTime::GetTotalTime() / RecordBase::RECORD_INTERVAL;
		if (ImGui::SliderInt("-", &time, 0, totalTime_int))
		{
			_time = time * RecordBase::RECORD_INTERVAL;

			for (RecordBase* record : RecordManager::RecordVector)
			{
				record->Select(_time);
			}
		
		}
		ImGui::Text(("Time: " + std::to_string(_time)).c_str());
		ImGui::Text(("Total: " + std::to_string(EngineTime::GetTotalTime())).c_str());
	}
	ImGui::End();
}

float TimeController::_time = 0;
bool TimeController::_isEditorON = false;