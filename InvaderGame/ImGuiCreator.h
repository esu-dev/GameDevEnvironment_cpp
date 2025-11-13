#pragma once

#include "framework.h"

class ImGuiCreator
{
public:
	static void Create(bool& outHasChanged, std::vector<std::string>& serializedDataVec);

private:
	static bool IsPointer(const std::string& value);
	static void PutPointerField();
};