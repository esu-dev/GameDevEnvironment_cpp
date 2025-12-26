#pragma once

#include "EngineBehaviour.h"

class GimmickTrigger;

class Shutter : public EngineBehaviour
{
public:
	NAME(Shutter)
	SERIALIZE3(EngineBehaviour,
		SERIALIZE_FIELD3(_openSpeed),
		SERIALIZE_FIELD3(_openDistance),
		SERIALIZE_FIELD3(_gimmickTrigger)
	)

	void Start() override;
	void Update() override;


private:
	float _openSpeed = 3.0f;
	float _openDistance = 3.0f; // ‚Ç‚ê‚­‚ç‚¢ˆÚ“®‚·‚é‚©

	GimmickTrigger* _gimmickTrigger = nullptr;

	float _movedDistance = 0.0f;
	bool _isOpening = false;
};