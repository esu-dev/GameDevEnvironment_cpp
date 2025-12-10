#pragma once

#include "framework.h"
#include "Scene.h"

class Object;

using namespace SceneManagement;

class SceneDataManager
{
public:
	static Object* GetInstance(const std::string& instanceID);
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();
	static void LoadGameObject(Scene* scene, const std::vector<std::string>& contentVector);
	static void LoadGameObjectClone(Scene* scene, const std::vector<std::string>& contentVector);
	static Scene* Load(std::string path);
	static Scene* Reload();
	static void Save();


private:
	struct InstanceData
	{
		Object* object = nullptr;
		std::vector<std::string> yamlVector;
	};

	static std::string _path;
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;

	static void DeserializeObject(Scene* scene, const std::vector<InstanceData>& instanceDataVector);
};

