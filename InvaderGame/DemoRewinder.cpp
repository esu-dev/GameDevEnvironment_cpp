#include "DemoRewinder.h"

#include "EngineTime.h"
#include "Transform.h"
#include "DemoClock.h"

void DemoRewinder::Start()
{
	if (_clockPrefab != nullptr)
	{
		_clock = Object::Instantiate(_clockPrefab);
		_clock->SetActive(false);
		_clock->GetTransform()->SetParent(this->GetTransform());
	}
}

void DemoRewinder::Update()
{
	if (!_isRewinding)
	{
		return;
	}


	// 位置の巻き戻し
	Transform* tr = this->GetTransform();
	if (tr == nullptr) return;
	tr->position = tr->position.GetRecord(_rewindTime);
	
	if (_rewindTime <= _recordStartTime)
	{
		_isRewinding = false;
		_clock->SetActive(false);
		return;
	}

	_rewindTime -= EngineTime::GetDeltaTime() * 1.5f;
}

void DemoRewinder::Record()
{
	// 連続で呼ばれる可能性あり
	if (!_isRecording)
	{
		_recordStartTime = EngineTime::GetTotalTime();
		_clock->GetComponent<DemoClock>()->Advance();
		_isRecording = true;
	}
	_clock->SetActive(true);
	_clock->GetComponent<DemoClock>()->SetRotationSpeed(240);
}

void DemoRewinder::Rewind()
{
	// 連続で呼ばれる可能性あり
	if (!_isRewinding)
	{
		_rewindTime = EngineTime::GetTotalTime();
	}
	_isRecording = false;
	_isRewinding = true;
	_clock->GetComponent<DemoClock>()->Rewind();
}