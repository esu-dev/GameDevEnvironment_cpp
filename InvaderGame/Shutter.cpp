#include "Shutter.h"
#include "GimmickTrigger.h"
#include "Collider2D.h"
#include "EngineTime.h"
#include "Transform.h"

void Shutter::Start()
{
	if (_gimmickTrigger != nullptr)
	{
		_gimmickTrigger->SetTriggerAction([this]() {
			this->_isOpening = true;

			// “–‚½‚è”»’è‚Ì–³Œø‰»
			Collider2D* collider = this->GetComponent<Collider2D>();
			if (collider != nullptr)
			{
				collider->enabled = false;
			}
		});
	}
}

void Shutter::Update()
{
	float move = _openSpeed * EngineTime::GetDelataTime();
	if (_isOpening)
	{
		_isOpening = false;

		// ˆÚ“®—Ê‚Ì’²®
		if (_movedDistance + move > _openDistance)
		{
			move = _openDistance - _movedDistance;
		}

		// ˆÚ“®
		this->GetTransform()->position.Get().y += move;
		_movedDistance += move;
	}
	else
	{
		// ˆÚ“®—Ê‚Ì’²®
		if (_movedDistance - move < 0)
		{
			move = _movedDistance;
		}

		// ˆÚ“®
		this->GetTransform()->position.Get().y -= move;
		_movedDistance -= move;
	}
}