#pragma once

#include "framework.h"
#include "FileManager.h"

class Object;
class GameObject;

class AssetManager
{
public:
	struct AssetFile
	{
		std::string instanceID;
		Object* object;
	};

	struct AssetFolder
	{
		std::unordered_map<std::string, std::variant<AssetFile*, AssetFolder*>> name2Datamp;
	};

	static void Initialize();
	static Object* GetInstance(const std::string& instanceID);
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();
	static const AssetFolder& GetAssetFolder();
	static void CreateAsset(const std::string& path, Object* object);
	static void CreateInstance(const std::string& directry, const std::string& fileName);
	static void SerializeGameObject(std::string& outSerializedData, GameObject* gameObject);
	static void SerializeGameObject(std::vector<std::string>& outSerializedData, GameObject* gameObject);

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
	static AssetFolder assetFolder;
};