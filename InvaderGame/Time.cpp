#include "Time.h"

const float Time::FixedDeltaTime = 1 / 60.0f;

void Time::Initialize()
{
	//_instance = new Time();
	GAMESYS.OnUpdateListener.AddListener([]()
		{
			ULONGLONG time = GetTickCount64();
			_deltaTime = (time - _updatedTime) / 1000.0f;
			_updatedTime = time;
		});
}

float Time::GetDelataTime()
{
	return _deltaTime;
}


ULONGLONG Time::_updatedTime = 0;
float Time::_deltaTime = 0;
