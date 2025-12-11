#include "TimeController.h"

#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "GameSystem.h"
#include "GameEngine.h"
#include "RecordManager.h"


void TimeController::Initialize()
{
	_isEditorON = false;
}

void TimeController::Update()
{
	// 
	static int width = 300;
	static int height = 200;

	static int backNum = 0;
	static int time = 0;


	// Editor‹N“®
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('T'))
	{
		_isEditorON = !_isEditorON;
		if (_isEditorON)
		{
			backNum = 0;
			EngineTime::TimeScale = 0;
		}
		else
		{
			float maxTime = 0;
			for (RecordBase* record : RecordManager::RecordVector)
			{
				float decidedTime = record->Decide(_time);
				maxTime = max(decidedTime, maxTime);
			}

			if (_time - maxTime > RecordBase::RECORD_INTERVAL)
			{
				EngineTime::TotalTime = _time;
			}
			else
			{
				EngineTime::TotalTime = maxTime;
			}

			EngineTime::TimeScale = 1;
		}
	}


	if (!_isEditorON)
	{
		return;
	}

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