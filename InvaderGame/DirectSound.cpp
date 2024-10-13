#include "DirectSound.h"

#include "AudioSource.h"

IDirectSound8* DirectSound::_directSoundDevice = nullptr;
IDirectSoundBuffer8* DirectSound::_dsSecondaryBuffer = nullptr;
std::vector<DirectSound::AudioData*> DirectSound::_audioDataVector = std::vector<DirectSound::AudioData*>();

void DirectSound::SetVolume(const AudioSource* audioSource, float volume)
{
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioSource == audioSource; });
	if (audioData != nullptr)
	{
		float max_dB = 20 * std::log10f(-DSBVOLUME_MIN);
		float v = -(std::powf(10, (1 - volume) * max_dB / 20));
		if (FAILED(audioData->dsSecondaryBuffer->SetVolume(v)))
		{
			Debug::Log(L"Failed set volume.");
		}
	}
}

void DirectSound::CreateSecondaryBuffer(AudioSource* audioSource)
{
	// �Z�J���h�o�b�t�@�����ɍ쐬����Ă��邩���m�F����
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioSource == audioSource; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Play(0, 0, 0);
		return;
	}


	// AudioClip����f�[�^�̎擾
	char* waveData = audioSource->GetAudioClip()->GetWaveData();
	DWORD waveSize = audioSource->GetAudioClip()->GetWaveSize();
	WAVEFORMATEX waveFormatEX = audioSource->GetAudioClip()->GetWAVEFORMATEX();


	// �Z�J���_���o�b�t�@�̍쐬
	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	dsBufferDesc.dwBufferBytes = waveSize;
	dsBufferDesc.dwReserved = 0;
	dsBufferDesc.lpwfxFormat = &waveFormatEX;
	dsBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* directSoundBuffer;

	// �T�E���h�o�b�t�@���쐬
	if (FAILED(_directSoundDevice->CreateSoundBuffer(&dsBufferDesc, &directSoundBuffer, NULL)))
	{
		Debug::Log(L"Failed CreateSoundBuffer method.");
		return;
	}

	// DirectSoundBuffer��8�ɕϊ�
	if (FAILED(directSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_dsSecondaryBuffer)))
	{
		Debug::Log(L"Failed QueryInterface method.");
		return;
	}
	directSoundBuffer->Release();


	// �Z�J���_���o�b�t�@��wav�f�[�^����������
	LPVOID write;
	DWORD length;

	// �����������b�N
	if (DS_OK == _dsSecondaryBuffer->Lock(0, 0, &write, &length, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		// �������̃R�s�[
		memcpy(write, waveData, length);
		_dsSecondaryBuffer->Unlock(write, length, NULL, 0);
		
		audioData = new AudioData();
		audioData->audioSource = audioSource;
		audioData->dsSecondaryBuffer = _dsSecondaryBuffer;
		_audioDataVector.push_back(audioData);
	}
}

void DirectSound::Play(AudioSource* audioSource)
{
	// �Z�J���h�o�b�t�@�����ɍ쐬����Ă��邩���m�F����
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioSource == audioSource; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Play(0, 0, 0);
	}
}

void DirectSound::Stop(AudioSource* audioSource)
{
	// �Z�J���h�o�b�t�@�����ɍ쐬����Ă��邩���m�F����
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioSource == audioSource; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Stop();
	}
}