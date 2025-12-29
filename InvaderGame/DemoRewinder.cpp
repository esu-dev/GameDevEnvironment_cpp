#include "DemoRewinder.h"

#include "EngineTime.h"
#include "Transform.h"

void DemoRewinder::Start()
{
	if (_clockPrefab != nullptr)
	{
		_clock = Object::Instantiate(_clockPrefab); // なぜか子オブジェクトが返ってきている
		_clock->SetActive(false);
		_clock->GetTransform()->SetParent(this->GetTransform());
	}
}

void DemoRewinder::Update()
{
	if (_isRecording)
	{
		// 回転
		_clock->GetTransform()->SetLocalRotation(Quaternion::Euler(_clock->GetTransform()->rotation.GetEulerAngles() - Vector3::forward * 10 * EngineTime::GetDeltaTime()));
	}

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
		_clock->GetTransform()->SetLocalRotation(Quaternion::Identity());
		return;
	}

	_rewindTime -= EngineTime::GetDeltaTime() * 1.5f;

	// 回転
	float r = _rewindStartTime - _recordStartTime;
	_clock->GetTransform()->SetLocalRotation(Quaternion::Euler(_clock->GetTransform()->rotation.GetEulerAngles() + Vector3::forward * 10 * EngineTime::GetDeltaTime() * 1.5f));
}

void DemoRewinder::Record()
{
	// 連続で呼ばれる可能性あり
	if (!_isRecording)
	{
		_recordStartTime = EngineTime::GetTotalTime();
	}
	_isRecording = true;
	_clock->SetActive(true);
}

void DemoRewinder::Rewind()
{
	if (!_isRewinding)
	{
		_rewindTime = EngineTime::GetTotalTime();
		_rewindStartTime = _rewindTime;
	}
	_isRecording = false;
	_isRewinding = true;
}