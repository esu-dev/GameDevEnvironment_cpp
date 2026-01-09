#pragma once

#include "framework.h"
#include "Object.h"

#include "GameObject.h"

class Transform;

#define RECORD_FIELD(v) \
	new RecordFuncData( \
		[](float t) -> void { v.Select(t); }, \
		[](float t) -> float { v.Decide(t); } \
	)

#define RECORD(p, ...) \
	void SelectTime(float time) override \
	{ \
		std::vector<RecordFuncData*> fV = { __VA_ARGS__ }; \
		for (auto f : fV) \
		{ \
			f->selectFunc(time); \
		} \
	} \
	\
	void DecideTime(float time) override \
	{ \
		std::vector<RecordFuncData*> fV = { __VA_ARGS__ }; \
		for (auto f : fV) \
		{ \
			f->decideFunc(time); \
		} \
	}

class Component : public Object
{
public:
	bool enabled = true;
	bool Started = false;
	GameObject* gameObject = nullptr;

	SERIALIZE3(Object,
		SERIALIZE_FIELD3(enabled),
		SERIALIZE_FIELD3(gameObject),
		SERIALIZE_FIELD3(Started),
	)

	virtual ~Component() { /*Debug::Log("Destructor is nor overrided. [%s]", GetName().c_str());*/ }
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnValidate() {}
	virtual void Start() {}
	virtual void Restart() { Start(); }
	virtual void Update() {}
	virtual void SelectTime(float time) {}
	virtual void DecideTime(float time) {}

	Transform* GetTransform();

	template<typename T> T* GetComponent() // template‚Í•ªŠ„ƒRƒ“ƒpƒCƒ‹‚·‚é‚Æ–Ê“|
	{
		return this->gameObject->GetComponent<T>();
	}


protected:
	struct RecordFuncData
	{
		std::function<void(float)> selectFunc;
		std::function<float(float)> decideFunc;

		RecordFuncData(std::function<void(float)> selectFunc, std::function<float(float)> decideFunc)
		{
			this->selectFunc = selectFunc;
			this->decideFunc = decideFunc;
		}
	};
};
