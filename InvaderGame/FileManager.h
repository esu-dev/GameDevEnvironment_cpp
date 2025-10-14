#pragma once

#include "framework.h"

class FileManager
{
public:
	static void Read(std::vector<std::string>& outContentVector, std::string path);
	static void Write(const std::string& path, const std::string& content);
};

