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
	std::vector<std::shared_ptr<SerializeFuncData>> GetSerializeFuncData() override {
		std::vector<std::shared_ptr<SerializeFuncData>> pV = RecordBase::GetSerializeFuncData(); std::vector<std::shared_ptr<SerializeFuncData>> sfdV = { std::make_shared<SerializeFuncData>([&](int indentNum) -> std::vector<std::string> { return SerializedClass::SerializeField("_variable", _variable, indentNum); }, [&](InstanceData* instanceData) -> void { DeserializeField(_variable, instanceData); }, [&]() -> FieldInfo { return SerializedClass::CreateFieldInfo("_variable", typeid(_variable).name(), _variable); }, [&](std::string name) -> void {}), std::make_shared<SerializeFuncData>([&](int indentNum) -> std::vector<std::string> { return SerializedClass::SerializeField("_timeVariableSetVector", _timeVariableSetVector, indentNum); }, [&](InstanceData* instanceData) -> void { DeserializeField(_timeVariableSetVector, instanceData); }, [&]() -> FieldInfo { return SerializedClass::CreateFieldInfo("_timeVariableSetVector", typeid(_timeVariableSetVector).name(), _timeVariableSetVector); }, [&](std::string name) -> void {}) }; pV.insert(pV.end(), sfdV.begin(), sfdV.end()); return pV;
	} std::vector<std::string> Serialize(const int indentNum = 1) override {
		std::vector<std::string> r; std::vector<std::shared_ptr<SerializeFuncData>> sfdV = GetSerializeFuncData(); for (auto& sfd : sfdV) {
			std::vector<std::string> s = sfd->serializeFunc(indentNum); r.insert(r.end(), s.begin(), s.end());
		} return r;
	} 
	int Deserialize(std::vector<std::string> v) override {
		std::vector<std::shared_ptr<SerializeFuncData>> sfdV = GetSerializeFuncData();
		InputValue3(v, sfdV);
		return 0;
	}

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

	T& GetRecord(float time)
	{
		for (int i = 0; i < _timeVariableSetVector.size(); i++)
		{
			if (time - _timeVariableSetVector[i].time < RECORD_INTERVAL)
			{
				return _variable = _timeVariableSetVector[i].variable;
			}
		}

		// 最初を返す
		return _timeVariableSetVector.front().variable;
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
	}
};