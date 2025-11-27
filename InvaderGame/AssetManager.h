#pragma once

#include "framework.h"
#include "FileManager.h"

class Object;

class AssetManager
{
public:
	static void Initialize();
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();
	static void CreateAsset(const std::string& path, Object* object);

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};