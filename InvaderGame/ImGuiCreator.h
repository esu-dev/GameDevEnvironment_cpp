#pragma once

#include "framework.h"

class ImGuiCreator
{
public:
	static bool PutPointerField(std::string& serializedData, const std::string& serializedVarName, const std::string& label, const std::string& instanceID);
	static void Create(bool& outHasChanged, std::vector<std::string>& serializedDataVec);

private:
	static bool IsArithmetic(std::smatch& outSmatch, const std::string& value);
	static bool IsPointer(std::string& instanceID, const std::string& value);
	static bool ArithmeticField(std::string& serializedData, const std::string& indent, const std::string& label, const std::string& value);
};