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
		SERIALIZE_FIELD3(_variable),
		SERIALIZE_FIELD3(_timeVariableSetVector)
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

		for (int i = (int)_timeVariableSetVector.size() - 1; i >= 0; i--)
		{
			if (time > _timeVariableSetVector[i].time)
			{
				_variable = _timeVariableSetVector[i].variable;

				return;
			}
		}
	}

	float Decide(float time) override
	{
		if (_timeVariableSetVector.size() == 0) return 0;

		for (int i = (int)_timeVariableSetVector.size() - 1; i >= 0; i--)
		{
			if (time > _timeVariableSetVector[i].time)
			{
				// 以降のデータを削除
				_timeVariableSetVector.erase(_timeVariableSetVector.begin() + i + 1, _timeVariableSetVector.end());

				return _timeVariableSetVector[i].time;
			}
		}

		return _timeVariableSetVector.back().time;
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

	T& GetRecord(float time)
	{
		for (int i = (int)_timeVariableSetVector.size() - 1; i >= 0; i--)
		{
			if (time > _timeVariableSetVector[i].time)
			{
				return _variable = _timeVariableSetVector[i].variable;
			}
		}

		// 最初を返す
		return _timeVariableSetVector.front().variable;
	}

	std::pair<std::pair<float, const T&>, std::pair<float, const T&>> GetTimeVarSetPair(float time)
	{
		for (int i = (int)_timeVariableSetVector.size() - 1; i >= 0; i--)
		{
			if (time > _timeVariableSetVector[i].time)
			{
				if (i + 1 < _timeVariableSetVector.size())
				{
					return {
						{ _timeVariableSetVector[i].time, _timeVariableSetVector[i].variable },
						{ _timeVariableSetVector[i + 1].time, _timeVariableSetVector[i + 1].variable }
					};
				}
				else
				{
					return {
						{ _timeVariableSetVector[i].time, _timeVariableSetVector[i].variable },
						{ _timeVariableSetVector[i].time, _timeVariableSetVector[i].variable}
					};
				}
			}
		}

		// 最初を返す
		return {
			{ _timeVariableSetVector[0].time, _timeVariableSetVector[0].variable },
			{ _timeVariableSetVector[1].time, _timeVariableSetVector[1].variable}
		};
	}

	operator T() { return _variable; }

	T operator+ (const T variable)
	{
		return _variable + variable;
	}

	T operator- (const T variable)
	{
		return _variable - variable;
	}

	void operator= (const T value)
	{
		_variable = value;

		UpdateValue();
	}

	void operator+= (const T variable)
	{
		_variable = _variable + variable;

		UpdateValue();
	}


private:
	struct TimeVariableSet : public SerializedClass
	{
		float time = 0;
		T variable;

		SERIALIZE3(SerializedClass,
			SERIALIZE_FIELD3(time),
			SERIALIZE_FIELD3(variable)
		)
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
		// 追加するほど時間がたっていないなら更新
		else if (_timeVariableSetVector.size() > 0)
		{
			_timeVariableSetVector.back().variable = _variable;
		}
	}
};