#pragma once

#include "framework.h"
#include "Time.h"

template <typename T>
class Record
{
public:
	operator T() { return _variable; }
	void operator= (const T variable);

private:
	struct TimeVariableSet
	{
		float time;
		T variable;
	};
	
	T _variable;
	std::vector<TimeVariableSet> _timeVariableSetVector;
};