#pragma once

#include "DirectX.h"

class AudioSource;

class DirectSound
{
public:
	static void Initialize(HWND windowHandle);
	static void SetVolume(const AudioSource* audioSource, float volume);
	static void CreateSecondaryBuffer(AudioSource* audioSource);
	static void Play(AudioSource* audioSource);
	static void Stop(AudioSource* audioSource);

private:
	struct AudioData
	{
		AudioSource* audioSource;
		IDirectSoundBuffer8* dsSecondaryBuffer;
	};

	static IDirectSound8* _directSoundDevice;
	static IDirectSoundBuffer8* _dsSecondaryBuffer;
	static std::vector<AudioData*> _audioDataVector;
};