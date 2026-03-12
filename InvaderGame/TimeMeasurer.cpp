#include "TimeMeasurer.h"

void TimeMeasurer::Start()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	_startTimeStack.push(startTime);
}

float TimeMeasurer::end()
{
	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration = endTime - _startTimeStack.top();
	_startTimeStack.pop();
	return duration.count();
}

std::stack<std::chrono::steady_clock::time_point> TimeMeasurer::_startTimeStack;