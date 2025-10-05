#include "Time.h"

#include "GameSystem.h"

const float Time::_fixedDeltaTime = 1 / 60.0f;
Property<float> Time::_totalTime{};
Property<float> Time::TimeScale{};

void Time::Initialize()
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

float Time::GetDelataTime()
{
	return _deltaTime;
}

float Time::GetFixedDeltaTime()
{
	return _fixedDeltaTime * TimeScale;
}

float Time::GetTotalTime()
{
	return _totalTime;
}

void Time::SetIsPause(bool isPause)
{
	_isPause = isPause;
}


bool Time::_isPause = false;
float Time::_deltaTime = 0;
