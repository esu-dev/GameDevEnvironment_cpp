#pragma once

#include "framework.h"
#include "Object.h"

class AssetManager
{
public:
	static void Initialize();

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};