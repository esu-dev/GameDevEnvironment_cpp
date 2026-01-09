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
	void StartRecording();
	void Rewind();

private:
	GameObject* _clockPrefab;

	bool _isRecording = false;
	Record<bool> _isRewinding = false;
	float _recordStartTime = 0.0f;
	Record<float> _rewindTime = 0.0f;
	GameObject* _clock;
};