#pragma once

#include "EngineFramework.h"

class AudioClip
{
public:
	AudioClip(const std::string& filePath);
	char* GetWaveData() const;
	DWORD GetWaveSize() const;
	WAVEFORMATEX GetWAVEFORMATEX() const;

private:
	char* _waveData;
	DWORD _waveSize;
	WAVEFORMATEX _waveFormatEX;
};