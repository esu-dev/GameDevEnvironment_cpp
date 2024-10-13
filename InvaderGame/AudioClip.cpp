#include "AudioClip.h"

AudioClip::AudioClip(const std::string& filePath)
{
	HMMIO handle_MMIO;
	MMIOINFO mmioInfo;

	// ゼロで初期化する
	memset(&mmioInfo, 0, sizeof(MMIOINFO));

	// wavファイルをオープン
	setlocale(LC_CTYPE, "jpn"); // 地域設定を日本語に設定。日本語はマルチバイト文字であるから変換する必要がある。
	wchar_t wFileName[256]; // wchar_tはワイド文字（ユニコード）を表すためのデータ型
	size_t ret; // size_t はオブジェクトのバイト数を表現できる程度に十分に大きい符号なし整数型。
	mbstowcs_s(&ret, wFileName, filePath.c_str(), 256);
	handle_MMIO = mmioOpen(wFileName, &mmioInfo, MMIO_READ);


	// RIFFチャンク検索
	MMRESULT mmResult;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmResult = mmioDescend(handle_MMIO, &riffChunk, NULL, MMIO_FINDRIFF);
	

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmResult = mmioDescend(handle_MMIO, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	

	// WAVEFORMATEX構造体格納
	DWORD formatSize = formatChunk.cksize;
	DWORD loadedSize = mmioRead(handle_MMIO, (HPSTR)&_waveFormatEX, formatSize);


	// RIFFチャンクに戻る
	mmioAscend(handle_MMIO, &formatChunk, 0);


	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = mmioDescend(handle_MMIO, &dataChunk, &riffChunk, MMIO_FINDCHUNK);


	// 音データ格納
	_waveData = new char[dataChunk.cksize]; // charは1Byteで扱いやすいから
	_waveSize = mmioRead(handle_MMIO, (HPSTR)_waveData, dataChunk.cksize);


	// mmioハンドルをクローズ
	mmioClose(handle_MMIO, 0);
}

char* AudioClip::GetWaveData() const
{
	return _waveData;
}

DWORD AudioClip::GetWaveSize() const
{
	return _waveSize;
}

WAVEFORMATEX AudioClip::GetWAVEFORMATEX() const
{
	return _waveFormatEX;
}