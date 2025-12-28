#include "DemoRewindTrigger.h"

#include "EngineTime.h"
#include "DemoRewinder.h"

void DemoRewindTrigger::OnEnable()
{
	_timer = 1;
}

void DemoRewindTrigger::OnTriggerStay2D(GameObject* other)
{
	if (DemoRewinder* rewinder = other->GetComponent<DemoRewinder>())
	{
		_rewindObjectVec.push_back(rewinder);
		rewinder->Record();
	}
}

void DemoRewindTrigger::Update()
{
	if (_timer <= 0)
	{
		this->gameObject->SetActive(false);
	}

	_timer -= EngineTime::GetDeltaTime();
}

void DemoRewindTrigger::Rewind()
{
	for (DemoRewinder* rewinder : _rewindObjectVec)
	{
		rewinder->Rewind();
	}
	_rewindObjectVec.clear();
}