#include "DemoRewindTrigger.h"

#include "EngineTime.h"
#include "Animator.h"
#include "BoxCollider2D.h"
#include "DemoRewinder.h"

void DemoRewindTrigger::OnEnable()
{
	
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

void DemoRewindTrigger::RecordTriggerOn()
{
	this->gameObject->SetActive(true);

	_timer = 1;

	// AnimationÄ¶
	this->GetComponent<Animator>()->Play("RecordAnimation");
}

void DemoRewindTrigger::Rewind()
{
	for (DemoRewinder* rewinder : _rewindObjectVec)
	{
		rewinder->Rewind();
	}
	_rewindObjectVec.clear();
}