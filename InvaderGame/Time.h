#pragma once

#include "framework.h"
#include "GameSystem.h"

class Time
{
public:
	static void Initialize();

	static double GetDelataTime();

private:
	static ULONGLONG _updatedTime;
	static double _deltaTime;
};