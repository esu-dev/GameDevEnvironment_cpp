#include "FileReader.h"

#include <fstream>
#include <iostream>
#include "Utility.h"

void FileReader::Read(std::vector<std::string>& outContentVector, std::string path)
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
