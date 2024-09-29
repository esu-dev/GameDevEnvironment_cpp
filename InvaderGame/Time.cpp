#include "Time.h"

void Time::Initialize()
{
	//_instance = new Time();
	GAMESYS.OnUpdateListener.AddListener([]()
		{
			ULONGLONG time = GetTickCount64();
			_deltaTime = (time - _updatedTime) / 1000.0;
			_updatedTime = time;
		});
}

double Time::GetDelataTime()
{
	return _deltaTime;
}


ULONGLONG Time::_updatedTime = 0;
double Time::_deltaTime = 0;
