#include "EngineTime.h"

#include "GameSystem.h"

const float EngineTime::_fixedDeltaTime = 1 / 60.0f;
Property<float> EngineTime::_totalTime{};
Property<float> EngineTime::TimeScale{};

void EngineTime::Initialize()
{
	static ULONGLONG updatedTime = GetTickCount64();

	TimeScale = 1;

	GAMESYS.OnUpdateListener.AddListener([]()
		{
			ULONGLONG time = GetTickCount64();
			_deltaTime = (time - updatedTime) / 1000.0f * TimeScale;
			updatedTime = time;

			_totalTime += _deltaTime;
		});
}

float EngineTime::GetDelataTime()
{
	return _deltaTime;
}

float EngineTime::GetFixedDeltaTime()
{
	return _fixedDeltaTime * TimeScale;
}

float EngineTime::GetTotalTime()
{
	return _totalTime;
}

void EngineTime::SetIsPause(bool isPause)
{
	_isPause = isPause;
}


bool EngineTime::_isPause = false;
float EngineTime::_deltaTime = 0;
