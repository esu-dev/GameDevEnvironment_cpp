#include "AudioClip.h"

AudioClip::AudioClip(const std::string& filePath)
{
	HMMIO handle_MMIO;
	MMIOINFO mmioInfo;

	// �[���ŏ���������
	memset(&mmioInfo, 0, sizeof(MMIOINFO));

	// wav�t�@�C�����I�[�v��
	setlocale(LC_CTYPE, "jpn"); // �n��ݒ����{��ɐݒ�B���{��̓}���`�o�C�g�����ł��邩��ϊ�����K�v������B
	wchar_t wFileName[256]; // wchar_t�̓��C�h�����i���j�R�[�h�j��\�����߂̃f�[�^�^
	size_t ret; // size_t �̓I�u�W�F�N�g�̃o�C�g����\���ł�����x�ɏ\���ɑ傫�������Ȃ������^�B
	mbstowcs_s(&ret, wFileName, filePath.c_str(), 256);
	handle_MMIO = mmioOpen(wFileName, &mmioInfo, MMIO_READ);


	// RIFF�`�����N����
	MMRESULT mmResult;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmResult = mmioDescend(handle_MMIO, &riffChunk, NULL, MMIO_FINDRIFF);
	

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmResult = mmioDescend(handle_MMIO, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	

	// WAVEFORMATEX�\���̊i�[
	DWORD formatSize = formatChunk.cksize;
	DWORD loadedSize = mmioRead(handle_MMIO, (HPSTR)&_waveFormatEX, formatSize);


	// RIFF�`�����N�ɖ߂�
	mmioAscend(handle_MMIO, &formatChunk, 0);


	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = mmioDescend(handle_MMIO, &dataChunk, &riffChunk, MMIO_FINDCHUNK);


	// ���f�[�^�i�[
	_waveData = new char[dataChunk.cksize]; // char��1Byte�ň����₷������
	_waveSize = mmioRead(handle_MMIO, (HPSTR)_waveData, dataChunk.cksize);


	// mmio�n���h�����N���[�Y
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