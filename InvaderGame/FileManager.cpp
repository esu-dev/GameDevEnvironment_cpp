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
		Debug::Log("ファイルを開けませんでした。(%s)", path);
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		outContentVector.push_back(line);
	}
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
