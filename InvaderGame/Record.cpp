#include "Record.h"

float RECORD_INTERVAL = 1;
/*
template <typename T>
Record<T>::operator T()
{
	return _variable;
}*/
/*
template <typename T>
void Record<T>::operator= (const T variable)
{
	_variable = variable;

	if (Time::GetTotalTime() % RECORD_INTERVAL <= Time::GetDelataTime())
	{
		TimeVariableSet timeVariableSet;
		timeVariableSet.time = Time::GetTotalTime();
		timeVariableSet.variable = _variable;
		_timeVariableSetVector.push_back(timeVariableSet);
	}
}*/