#include "Record.h"

template <typename T>
void Record<T>::operator= (const T variable)
{
	_variable = variable;

	TimeVariableSet timeVariableSet;
	timeVariableSet.time = Time::GetTotalTime();
	//_timeVariableSetVector.push_back()
}