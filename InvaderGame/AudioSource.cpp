#include "AudioSource.h"

void AudioSource::SetVolume(float volume)
{
	_volume.SetVolume(volume);
	DirectSound::SetVolume(this, _volume.GetVolume());
}

void AudioSource::SetAudioClip(AudioClip* audioClip)
{
	_clip = audioClip;
	DirectSound::CreateSecondaryBuffer(this);
	DirectSound::SetVolume(this, 0.25f);
}

const AudioClip* AudioSource::GetAudioClip()
{
	return _clip;
}

void AudioSource::Play()
{
	DirectSound::Play(this);
}

void AudioSource::Stop()
{
	DirectSound::Stop(this);
}

float AudioSource::Volume::GetVolume()
{
	return _volume;
}

void AudioSource::Volume::SetVolume(float volume)
{
	_volume = volume;

	if (_volume < 0)
	{
		_volume = 0;
	}
	else if (_volume > 1)
	{
		_volume = 1;
	}
}