#pragma once

#include "framework.h"
#include "DirectX.h"
#include "Component.h"

class AudioSource : public Component
{
public:
	void SetVolume(float volume);
	void SetAudioClip(AudioClip* audioClip);
	const AudioClip* GetAudioClip();
	void Play();
	void Stop();

private:
	class Volume
	{
	public:
		float GetVolume();
		void SetVolume(float volume);

	private:
		float _volume;
	};

	Volume _volume;
	AudioClip* _clip;
};