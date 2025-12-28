#include "Shutter.h"
#include "Vector3.h"
#include "GimmickTrigger.h"
#include "Collider2D.h"
#include "EngineTime.h"
#include "Transform.h"

void Shutter::Start()
{
	_startPositionY = this->GetTransform()->position.Get().y;

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
	float move = _openSpeed * EngineTime::GetDeltaTime();
	float currentPositionY = this->GetTransform()->position.Get().y;
	if (_isOpening)
	{
		_isOpening = false;

		// ˆÚ“®—Ê‚Ì’²®
		float targetPositionY = _startPositionY + _openDistance;
		if (currentPositionY + move > targetPositionY)
		{
			move = targetPositionY - currentPositionY;
		}

		// ˆÚ“®
		this->GetTransform()->position.Get().y += move;
	}
	else
	{
		// ˆÚ“®—Ê‚Ì’²®
		if (currentPositionY - move < _startPositionY)
		{
			move = currentPositionY - _startPositionY;
		}

		// ˆÚ“®
		this->GetTransform()->position.Get().y -= move;
	}
}