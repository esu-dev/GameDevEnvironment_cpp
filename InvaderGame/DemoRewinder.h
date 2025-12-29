#pragma once

#include "EngineBehaviour.h"

class DemoRewinder : public EngineBehaviour
{
public:
	NAME(DemoRewinder)
	SERIALIZE3(EngineBehaviour,
		SERIALIZE_FIELD3(_clockPrefab)
	)

	void Start() override;
	void Update() override;
	void Record();
	void Rewind();

private:
	GameObject* _clockPrefab;

	bool _isRecording = false;
	bool _isRewinding = false;
	float _recordStartTime = 0.0f;
	float _rewindTime = 0.0f;
	float _rewindStartTime;
	GameObject* _clock;
};