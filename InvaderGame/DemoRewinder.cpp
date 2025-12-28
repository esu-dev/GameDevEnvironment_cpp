#include "DemoRewinder.h"

#include "EngineTime.h"
#include "Transform.h"

void DemoRewinder::Update()
{
	if (!_isRewinding)
	{
		return;
	}

	Transform* tr = this->GetTransform();
	if (tr == nullptr) return;

	tr->position = tr->position.GetRecord(_rewindTime);


	if (_rewindTime <= _recordStartTime)
	{
		_isRewinding = false;
		return;
	}

	_rewindTime -= EngineTime::GetDeltaTime() * 1.25f;
}

void DemoRewinder::Record()
{
	_recordStartTime = EngineTime::GetTotalTime();
}

void DemoRewinder::Rewind()
{
	if (!_isRewinding)
	{
		_rewindTime = EngineTime::GetTotalTime();
	}
	_isRewinding = true;
}