#pragma once

#include "Utility.h"

class EngineTime
{
public:
	static Property<float> TotalTime;
	static Property<float> TimeScale;

	static void Initialize();

	static float GetDeltaTime();
	static float GetFixedDeltaTime();
	static float GetTotalTime();
	static void SetIsPause(bool isPause);

private:
	static const float _fixedDeltaTime;

	static bool _isPause;
	static float _deltaTime;
};