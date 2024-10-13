#pragma once

#include "framework.h"

class AudioClip
{
public:
	AudioClip(const std::string& filePath);
	char* GetWaveData();
	DWORD GetWaveSize();
	WAVEFORMATEX& GetWAVEFORMATEX();

private:
	char* _waveData;
	DWORD _waveSize;
	WAVEFORMATEX _waveFormatEX;
};