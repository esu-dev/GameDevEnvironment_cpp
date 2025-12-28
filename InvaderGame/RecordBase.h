#pragma once

#include "SerializedClass.h"

class RecordBase : public SerializedClass
{
public:
	static constexpr float RECORD_INTERVAL = 0.015f;

	virtual void Initialize() = 0;
	virtual void Select(float time) = 0;
	virtual float Decide(float time) = 0;
};