#pragma once

#include <stdlib.h>
#include <vector>
#include "Time.h"
#include "RecordBase.h"
#include "RecordManager.h"

template <typename T>
class Record : public RecordBase
{
public:
	void Back() override
	{
		_variable = _timeVariableSetVector[0].variable;
	}

	Record()
	{
		UpdateValue();
		RecordManager::RecordVector.push_back(this);
	}

	Record(T value)
	{
		_variable = value;
		UpdateValue();
		RecordManager::RecordVector.push_back(this);
	}

	~Record()
	{
		// リストから削除
	}

	const T& Get() { return _variable; }

	operator T() { return _variable; } // こっちは参照型じゃなくて良いのかよ？

	T operator- (const T variable)
	{
		return _variable - variable;
	}

	void operator= (const T variable)
	{
		_variable = variable;

		UpdateValue();
	}

	void operator+= (const T variable)
	{
		_variable = _variable + variable;

		UpdateValue();
	}


private:
	struct TimeVariableSet
	{
		float time;
		T variable;
	};
	
	T _variable;
	std::vector<TimeVariableSet> _timeVariableSetVector;

	void UpdateValue()
	{
		static const float RECORD_INTERVAL = 1;

		float _lastTime = _timeVariableSetVector.size() == 0 ? 0 : _timeVariableSetVector.back().time;
		if (_timeVariableSetVector.size() == 0 || Time::GetTotalTime() - _lastTime >= RECORD_INTERVAL)
		{
			TimeVariableSet timeVariableSet;
			timeVariableSet.time = Time::GetTotalTime();
			timeVariableSet.variable = _variable;
			_timeVariableSetVector.push_back(timeVariableSet);
		}
	}
};