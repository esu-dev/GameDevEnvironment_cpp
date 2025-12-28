#pragma once

#include "Utility.h"

class EngineTime
{
public:
	static float TimeScale;
	
	static void Initialize();
	static void Update();

	static float GetDeltaTime();
	static void ResetDeltaTime();
	static float GetFixedDeltaTime();
	static float GetTotalTime();
	static void SetTotalTime(float totalTime);
	static void SetIsPause(bool isPause);


private:
	static const float _fixedDeltaTime;

	static bool _isPause;
	static float _deltaTime;
	static float _totalTime;
	static ULONGLONG updatedTime;
};