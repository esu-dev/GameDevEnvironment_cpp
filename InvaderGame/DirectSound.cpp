#include "DirectSound.h"

IDirectSound8* DirectSound::_directSoundDevice = nullptr;
IDirectSoundBuffer8* DirectSound::_dsSecondaryBuffer = nullptr;
std::vector<DirectSound::AudioData*> DirectSound::_audioDataVector = std::vector<DirectSound::AudioData*>();

void DirectSound::Play(AudioClip* audioClip)
{
	// セカンドバッファが既に作成されているかを確認する
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioClip == audioClip; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Play(0, 0, 0);
		return;
	}


	// AudioClipからデータの取得
	char* waveData = audioClip->GetWaveData();
	DWORD waveSize = audioClip->GetWaveSize();
	WAVEFORMATEX& waveFormatEX = audioClip->GetWAVEFORMATEX();

	
	// セカンダリバッファの作成
	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = 0;
	dsBufferDesc.dwBufferBytes = waveSize;
	dsBufferDesc.dwReserved = 0;
	dsBufferDesc.lpwfxFormat = &waveFormatEX;
	dsBufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer* directSoundBuffer;

	// サウンドバッファを作成
	if (FAILED(_directSoundDevice->CreateSoundBuffer(&dsBufferDesc, &directSoundBuffer, NULL)))
	{
		Debug::Log(L"Failed CreateSoundBuffer method.");
		return;
	}

	// DirectSoundBufferを8に変換
	if (FAILED(directSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&_dsSecondaryBuffer)))
	{
		Debug::Log(L"Failed QueryInterface method.");
		return;
	}
	directSoundBuffer->Release();


	// セカンダリバッファにwavデータを書き込む
	LPVOID write;
	DWORD length;

	// メモリをロック
	if (DS_OK == _dsSecondaryBuffer->Lock(0, 0, &write, &length, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		// メモリのコピー
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
	// セカンドバッファが既に作成されているかを確認する
	AudioData* audioData = std_extension::Find<AudioData*>(_audioDataVector, [&](AudioData* x) { return x->audioClip == audioClip; });
	if (audioData != nullptr)
	{
		audioData->dsSecondaryBuffer->Stop();
	}
}