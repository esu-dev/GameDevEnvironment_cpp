#pragma once

#include "framework.h"

class Object;

class AssetManager
{
public:
	static void Initialize();
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};