#pragma once

#include <stdlib.h>
#include <vector>
#include "Time.h"

template <typename T>
class Record
{
public:
	static const float RECORD_INTERVAL;

	operator T() { return _variable; }
	void operator= (const T variable)
	{
		_variable = variable;

		if (true)
		{
			TimeVariableSet timeVariableSet;
			timeVariableSet.time = Time::GetTotalTime();
			timeVariableSet.variable = _variable;
			_timeVariableSetVector.push_back(timeVariableSet);
		}
	}

private:
	struct TimeVariableSet
	{
		float time;
		T variable;
	};
	
	T _variable;
	std::vector<TimeVariableSet> _timeVariableSetVector;
};