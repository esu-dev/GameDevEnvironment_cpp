#include "FileManager.h"

#include <fstream>
#include <iostream>
#include "Utility.h"

#include "GameEngine.h"

void FileManager::Read(std::vector<std::string>& outContentVector, std::string path)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		Debug::Log("ファイルを開けませんでした。(%s)", path.c_str());
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		outContentVector.push_back(line);
	}
}

std::vector<std::string> FileManager::GetAllPath(std::string directry, std::string extension)
{
	std::vector<std::string> pathVector;

	std::string wildCard = directry + std::string("*.") + extension;

	// stringをLPWSTRに変換する
	int length = MultiByteToWideChar(CP_UTF8, 0, wildCard.c_str(), -1, nullptr, 0);
	WCHAR* buffer = new WCHAR[length]; // 配列のメモリ動的割り当てかな
	MultiByteToWideChar(CP_UTF8, 0, wildCard.c_str(), -1, buffer, length);
	LPWSTR wildCard_lpwstr = buffer;

	WIN32_FIND_DATA win32FindData;
	HANDLE hFindFile = FindFirstFileW(wildCard_lpwstr, &win32FindData);

	// ファイルが見つからなかった場合
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		Debug::Log("file was not found.");
		return pathVector;
	}

	// ファイルを読み込み続ける
	do
	{
		Debug::Log("%s", win32FindData.cFileName); // 最初の一文字しか取れていない
	} while (FindNextFileW(hFindFile, &win32FindData));


	FindClose(hFindFile);

	return pathVector;
}

void FileManager::Write(const std::string& path, const std::string& content)
{
	std::ofstream file(path);

	if (!file)
	{
		Debug::Log(L"ファイルを開けませんでした。");
		return;
	}

	file << content << std::endl;

	file.close();

	Debug::Log(L"ファイル書き込みが完了しました。");
}
