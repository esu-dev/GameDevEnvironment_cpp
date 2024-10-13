#include "DirectSound.h"

IDirectSound8* DirectSound::_directSoundDevice = nullptr;
IDirectSoundBuffer8* DirectSound::_dsSecondaryBuffer = nullptr;
std::vector<DirectSound::AudioData*> DirectSound::_audioDataVector = std::vector<DirectSound::AudioData*>();

void DirectSound::Play(AudioClip* audioClip)
{
	// �Z�J���h�o�b�t�@�����ɍ쐬����Ă��邩���m�F����
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioClip == audioClip; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Play(0, 0, 0);
		return;
	}


	// AudioClip����f�[�^�̎擾
	char* waveData = audioClip->GetWaveData();
	DWORD waveSize = audioClip->GetWaveSize();
	WAVEFORMATEX& waveFormatEX = audioClip->GetWAVEFORMATEX();

	
	// �Z�J���_���o�b�t�@�̍쐬
	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = 0;
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
		_dsSecondaryBuffer->Play(0, 0, 0);

		audioData = new AudioData();
		audioData->audioClip = audioClip;
		audioData->dsSecondaryBuffer = _dsSecondaryBuffer;
		_audioDataVector.push_back(audioData);
	}
}

void DirectSound::Stop(AudioClip* audioClip)
{
	// �Z�J���h�o�b�t�@�����ɍ쐬����Ă��邩���m�F����
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioClip == audioClip; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Stop();
	}
}