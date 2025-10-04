#include "Time.h"

const float Time::FixedDeltaTime = 1 / 60.0f;

void Time::Initialize()
{
	_updatedTime = GetTickCount64();

	GAMESYS.OnUpdateListener.AddListener([]()
		{
			ULONGLONG time = GetTickCount64();
			_deltaTime = (time - _updatedTime) / 1000.0f;
			_updatedTime = time;

			_totalTime += _deltaTime;
		});
}

float Time::GetDelataTime()
{
	return _deltaTime;
}

float Time::GetTotalTime()
{
	return _totalTime;
}


ULONGLONG Time::_updatedTime = 0;
float Time::_deltaTime = 0;
float Time::_totalTime = 0;
