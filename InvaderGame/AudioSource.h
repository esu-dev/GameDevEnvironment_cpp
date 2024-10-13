#pragma once

#include "framework.h"
#include "DirectX.h"
#include "Component.h"

class AudioSource : public Component
{
public:
	void SetAudioClip(AudioClip* audioClip);
	void Play();
	void Stop();

private:
	AudioClip* _clip;
};