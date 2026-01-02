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
	
	// 線形補完
	auto timeVerSetPair = tr->position.GetTimeVarSetPair(_rewindTime);
	float timeA = timeVerSetPair.first.first;
	float timeB = timeVerSetPair.second.first;
	Vector3 posA = timeVerSetPair.first.second;
	Vector3 posB = timeVerSetPair.second.second;

	// 時間の差を求める
	float timeRange = timeB - timeA;

	// 最新データで補完できないなら
	if (timeRange == 0)
	{
		tr->position = posA;
	}
	else
	{
		// 補完時間位置を求める
		float t = (_rewindTime - timeA) / timeRange;

		// 位置の差を求める
		Vector3 positionRange = posB - posA;

		// 補完した位置を求める
		Vector3 supprementedPos = posA + positionRange * t;

		tr->position = supprementedPos;
	}


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
	_clock->GetComponent<DemoClock>()->SetRotationSpeed(360);
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