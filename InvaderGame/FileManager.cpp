#include "FileManager.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "Utility.h"

#include "GameEngine.h"

bool FileManager::Exist(const std::string& path)
{
	std::ifstream file;
	file.open(path);
	bool isNotExist = !file;
	if (isNotExist)
	{
		file.close();
	}
	return !isNotExist;
}


bool FileManager::Exist(const std::wstring& path)
{
	std::ifstream file;
	file.open(path);
	bool isNotExist = !file;
	if (isNotExist)
	{
		file.close();
	}
	return !isNotExist;
}

void FileManager::Read(std::vector<std::string>& outContentVector, std::string path)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		Debug::Log("File could not opened. (%s)", path.c_str());
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		outContentVector.push_back(line);
	}

	file.close();
}

void FileManager::Read(std::vector<std::string>& outContentVector, std::wstring path)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		Debug::Log("File could not opened. (%s)", path.c_str());
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		outContentVector.push_back(line);
	}

	file.close();
}

std::vector<std::string> FileManager::GetAllFileName(std::string directry, std::string extension)
{
	std::vector<std::string> pathVector;

	std::string wildCard = directry + std::string("*.") + extension;

	WIN32_FIND_DATAA win32FindData;
	HANDLE hFindFile = FindFirstFileA(wildCard.c_str(), &win32FindData);

	// ファイルが見つからなかった場合
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		Debug::Log("file(%s) was not found.[FileManager::GetAllFileName()]", wildCard.c_str());
		return pathVector;
	}

	// ファイルを読み込み続ける
	do
	{
		std::string fileName = win32FindData.cFileName;
		//Debug::Log("%s", fileName.c_str()); // 最初の一文字しか取れていない
		pathVector.push_back(fileName);
	} while (FindNextFileA(hFindFile, &win32FindData));


	FindClose(hFindFile);

	return pathVector;
}

std::vector<std::wstring> FileManager::GetAllFileName(std::wstring directry, std::wstring extension)
{
	std::vector<std::wstring> pathVector;

	std::wstring wildCard_wstring = directry + L"*." + extension;
	LPCWSTR wildCard = wildCard_wstring.c_str();

	WIN32_FIND_DATA win32FindData;
	HANDLE hFindFile = FindFirstFileW(wildCard, &win32FindData);

	// ファイルが見つからなかった場合
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		Debug::Log("file was not found.[FileManager::GetAllFileName()]");
		return pathVector;
	}

	// ファイルを読み込み続ける
	do
	{
		std::wstring fileName = win32FindData.cFileName;
		//Debug::Log("%s", fileName.c_str()); // 最初の一文字しか取れていない
		pathVector.push_back(fileName);
	} while (FindNextFileW(hFindFile, &win32FindData));


	FindClose(hFindFile);

	return pathVector;
}

std::vector<std::string> FileManager::GetAllFolderName(std::string directry)
{
	std::vector<std::string> pathVector;

	std::string wildCard = directry + std::string("*");

	WIN32_FIND_DATAA win32FindData;
	HANDLE hFindFile = FindFirstFileA(wildCard.c_str(), &win32FindData);

	// ファイルが見つからなかった場合
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		Debug::Log("file was not found.[FileManager::GetAllFolderName()]");
		return pathVector;
	}

	// ファイルを読み込み続ける
	do
	{
		std::string fileName = win32FindData.cFileName;

		// 拡張子が付いていないもののみ加える
		std::smatch smatch;
		if (std::regex_match(fileName, smatch, std::regex(R"([^.]+)")))
		{
			pathVector.push_back(fileName);
		}
	} while (FindNextFileA(hFindFile, &win32FindData));


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

	time_t now = time(nullptr);
	tm lt;
	localtime_s(&lt, &now);
	char timeBuf[64];
	strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &lt);
	Debug::Log("[%s] Writing file is completed. (path: %s)", timeBuf, path.c_str());
}

void FileManager::Write(const std::wstring& path, const std::string& content)
{
	std::ofstream file(path);

	if (!file)
	{
		Debug::Log(L"ファイルを開けませんでした。[FileManager::Write()]");
		return;
	}

	file << content << std::endl;

	file.close();

	time_t now = time(nullptr);
	tm lt;
	localtime_s(&lt, &now);
	wchar_t timeBuf[64];
	wcsftime(timeBuf, sizeof(timeBuf), L"%H:%M:%S", &lt);
	Debug::Log(L"[%s] ファイル書き込みが完了しました。", timeBuf);
}

std::string FileManager::RemoveExtension(const std::string& path)
{
	std::string fileName;
	std::smatch wsmatch;
	if (std::regex_match(path, wsmatch, std::regex(R"((.+)\.\w+)")))
	{
		fileName = wsmatch[1].str();
	}

	return fileName;
}