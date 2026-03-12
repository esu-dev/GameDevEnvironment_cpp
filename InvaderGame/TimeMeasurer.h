#pragma once

#include <stack>
#include "EngineFramework.h"

class TimeMeasurer
{
public:
	static void Start();
	static float end();

private:
	static std::stack<std::chrono::steady_clock::time_point> _startTimeStack;
};