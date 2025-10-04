#include "Time.h"

const float Time::_fixedDeltaTime = 1 / 60.0f;
Property<float> Time::TimeScale{};

void Time::Initialize()
{
	TimeScale = 1;
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
	return _deltaTime * TimeScale;
}

float Time::GetFixedDeltaTime()
{
	return _fixedDeltaTime * TimeScale;
}

float Time::GetTotalTime()
{
	return _totalTime;
}


ULONGLONG Time::_updatedTime = 0;
float Time::_deltaTime = 0;
float Time::_totalTime = 0;
