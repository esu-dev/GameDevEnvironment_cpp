#include "DemoClock.h"

#include "Quaternion.h"
#include "EngineTime.h"
#include "Transform.h"

void DemoClock::Update()
{
	if (_isAdvance)
	{
		_secondHandParent->GetTransform()->SetLocalRotation(Quaternion::Euler(_secondHandParent->GetTransform()->rotation.Get().GetEulerAngles() - Vector3::forward * _rotationSpeed * EngineTime::GetDeltaTime()));
		_minuteHandParent->GetTransform()->SetLocalRotation(Quaternion::Euler(_minuteHandParent->GetTransform()->rotation.Get().GetEulerAngles() - Vector3::forward * _rotationSpeed / 12 * EngineTime::GetDeltaTime()));
	}
	else
	{
		_secondHandParent->GetTransform()->SetLocalRotation(Quaternion::Euler(_secondHandParent->GetTransform()->rotation.Get().GetEulerAngles() + Vector3::forward * _rotationSpeed * EngineTime::GetDeltaTime() * 1.5f));
		_minuteHandParent->GetTransform()->SetLocalRotation(Quaternion::Euler(_minuteHandParent->GetTransform()->rotation.Get().GetEulerAngles() + Vector3::forward * _rotationSpeed / 12 * EngineTime::GetDeltaTime() * 1.5f));
	}
}

void DemoClock::SetRotationSpeed(float rotationSpeed)
{
	_rotationSpeed = rotationSpeed;
}

void DemoClock::Advance()
{
	_isAdvance = true;
	_secondHandParent->GetTransform()->SetLocalRotation(Quaternion::Identity());
	_minuteHandParent->GetTransform()->SetLocalRotation(Quaternion::Identity());
}

void DemoClock::Rewind()
{
	_isAdvance = false;
}