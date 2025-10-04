#pragma once

#include "framework.h"
#include "GameSystem.h"

class Time
{
public:
	// privateÇ…ïœçXó\íË
	static const float _fixedDeltaTime;

	static Property<float> TimeScale;

	static void Initialize();

	static float GetDelataTime();
	static float GetFixedDeltaTime();
	static float GetTotalTime();

private:
	static ULONGLONG _updatedTime;
	static float _deltaTime;
	static float _totalTime;
};