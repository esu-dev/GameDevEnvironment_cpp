#include "Shutter.h"
#include "Vector3.h"
#include "GimmickTrigger.h"
#include "Collider2D.h"
#include "EngineTime.h"
#include "Transform.h"

void Shutter::Start()
{
	// デシリアライズで上書きされないように
	if (_startPositionY == 0)
	{
		_startPositionY = this->GetTransform()->position.Get().y;
	}

	if (_gimmickTrigger != nullptr)
	{
		_gimmickTrigger->SetTriggerAction([this]() {
			this->_isOpening = true;
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

		// 移動量の調整
		float targetPositionY = _startPositionY + _openDistance;
		if (currentPositionY + move > targetPositionY)
		{
			move = targetPositionY - currentPositionY;
		}

		// 移動
		this->GetTransform()->position += Vector3::up * move;
	}
	else
	{
		// 移動量の調整
		if (currentPositionY - move < _startPositionY)
		{
			move = currentPositionY - _startPositionY;
		}

		// 移動
		this->GetTransform()->position -= Vector3::up * move;
	}
}