#pragma once

#include "framework.h"
#include "GameSystem.h"

class Time
{
public:
	static const float FixedDeltaTime;
	static void Initialize();

	static float GetDelataTime();

private:
	static ULONGLONG _updatedTime;
	static float _deltaTime;
};