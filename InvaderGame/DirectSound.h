#pragma once

#include "framework.h"
#include "DirectX.h"

class DirectSound
{
public:
	static void Initialize(HWND windowHandle)
	{
		// サウンドデバイスの作成
		if (FAILED(DirectSoundCreate8(NULL, (LPDIRECTSOUND8*)&_directSoundDevice, NULL)))
		{
			Debug::Log(L"DirectSound could not be created.");
			return;
		}

		_directSoundDevice->SetCooperativeLevel(windowHandle, DSSCL_NORMAL);
	}
	static void Play(AudioClip* audioClip);
	static void Stop(AudioClip* audioClip);

private:
	struct AudioData
	{
		AudioClip* audioClip;
		IDirectSoundBuffer8* dsSecondaryBuffer;
	};

	static IDirectSound8* _directSoundDevice;
	static IDirectSoundBuffer8* _dsSecondaryBuffer;
	static std::vector<AudioData*> _audioDataVector;
};