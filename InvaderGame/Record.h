#pragma once

#include <stdlib.h>
#include <vector>
#include "EngineTime.h"
#include "RecordBase.h"
#include "RecordManager.h"

template <typename T>
class Record : public RecordBase
{
public:
	SERIALIZE3(RecordBase,
		SERIALIZE_FIELD3(_variable)
	)

	void Initialize() override
	{
		_timeVariableSetVector.clear();

		TimeVariableSet timeVariableSet;
		timeVariableSet.time = EngineTime::GetTotalTime();
		timeVariableSet.variable = _variable;
		_timeVariableSetVector.push_back(timeVariableSet);
	}

	void Select(float time) override
	{
		if (_timeVariableSetVector.size() == 0) return;

		for (int i = 0; i < (int)_timeVariableSetVector.size(); i++)
		{
			if (time - _timeVariableSetVector[i].time < RECORD_INTERVAL)
			{
				_variable = _timeVariableSetVector[i].variable;

				return;
			}
		}
	}

	float Decide(float time) override
	{
		if (_timeVariableSetVector.size() == 0) return 0;

		for (int i = 0; i < (int)_timeVariableSetVector.size(); i++)
		{
			if (time - _timeVariableSetVector[i].time < RECORD_INTERVAL)
			{
				// 以降のデータを削除
				_timeVariableSetVector.erase(_timeVariableSetVector.begin() + i + 1, _timeVariableSetVector.end());

				return _timeVariableSetVector[i].time;
			}
		}

		return 0;
	}

	Record()
	{
		_variable = T();
		RecordManager::RecordVector.push_back(this);
	}

	Record(T value)
	{
		_variable = value;
		RecordManager::RecordVector.push_back(this);
	}

	~Record()
	{
		// リストから削除
		std_extension::Remove(RecordManager::RecordVector, (RecordBase*)this);
	}

	T& Get() { return _variable; }

	operator T() { return _variable; }

	T operator+ (const T variable)
	{
		return _variable + variable;
	}

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
		float time = 0;
		T variable;
	};
	

	T _variable;
	std::vector<TimeVariableSet> _timeVariableSetVector;

	void UpdateValue()
	{
		float _lastTime = _timeVariableSetVector.size() == 0 ? 0 : _timeVariableSetVector.back().time;
		if (EngineTime::GetTotalTime() - _lastTime >= RECORD_INTERVAL)
		{
			TimeVariableSet timeVariableSet;
			timeVariableSet.time = EngineTime::GetTotalTime();
			timeVariableSet.variable = _variable;
			_timeVariableSetVector.push_back(timeVariableSet);
		}
	}
};