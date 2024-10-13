#include "AudioSource.h"

void AudioSource::SetAudioClip(AudioClip* audioClip)
{
	_clip = audioClip;
}

void AudioSource::Play()
{
	DirectSound::Play(_clip);
}

void AudioSource::Stop()
{
	DirectSound::Stop(_clip);
}