#pragma once

class RecordBase
{
public:
	static constexpr float RECORD_INTERVAL = 1;

	virtual void Select(float time) = 0;
	virtual void Decide(float time) = 0;
};