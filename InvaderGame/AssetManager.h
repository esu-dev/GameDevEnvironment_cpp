#pragma once

#include "framework.h"
#include "FileManager.h"

class Object;
class GameObject;

class AssetManager
{
public:
	static void Initialize();
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();
	static void CreateAsset(const std::string& path, Object* object);
	static void CreateInstance(const std::string& path);
	static void SerializeGameObject(std::string& outSerializedData, GameObject* gameObject);

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};