#pragma once

#include "framework.h"

class FileManager
{
public:
	static bool Exist(const std::wstring& path);
	static void Read(std::vector<std::string>& outContentVector, std::string path);
	static void Read(std::vector<std::string>& outContentVector, std::wstring path);
	static std::vector<std::wstring> GetAllFileName(std::string directry, std::string extension);
	static std::vector<std::wstring> GetAllFileName(std::wstring directry, std::wstring extension);
	static void Write(const std::string& path, const std::string& content);
	static void Write(const std::wstring& path, const std::string& content);
};

