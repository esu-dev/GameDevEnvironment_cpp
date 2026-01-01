#include "EngineTime.h"

#include "GameSystem.h"

const float EngineTime::_fixedDeltaTime = 1 / 60.0f;
float EngineTime::TimeScale = 1;

void EngineTime::Initialize()
{
	TimeScale = 1;
	ResetDeltaTime();

	GAMESYS.OnUpdateListener.AddListener([]()
		{
			ULONGLONG time = GetTickCount64();
			_deltaTime = (time - updatedTime) / 1000.0f * TimeScale;
			_engineDeltaTime = (time - updatedTime) / 1000.0f;
			updatedTime = time;

			_totalTime += _deltaTime;
		});
}

void EngineTime::Update()
{
	/*static ULONGLONG updatedTime = GetTickCount64();

	ULONGLONG time = GetTickCount64();
	_deltaTime = (time - updatedTime) / 1000.0f * TimeScale;
	updatedTime = time;

	_totalTime += _deltaTime;*/
}

float EngineTime::GetDeltaTime()
{
	return _deltaTime;
}

void EngineTime::ResetDeltaTime()
{
	updatedTime = GetTickCount64();
}

float EngineTime::GetFixedDeltaTime()
{
	return _fixedDeltaTime * TimeScale;
}

float EngineTime::GetEngineDeltaTime()
{
	return _engineDeltaTime;
}

float EngineTime::GetTotalTime()
{
	return _totalTime;
}

void EngineTime::SetTotalTime(float totalTime)
{
	_totalTime = totalTime;
}

void EngineTime::SetIsPause(bool isPause)
{
	_isPause = isPause;
}


bool EngineTime::_isPause = false;
float EngineTime::_deltaTime = 0;
float EngineTime::_engineDeltaTime = 0;
float EngineTime::_totalTime = 0;
ULONGLONG EngineTime::updatedTime = 0;
