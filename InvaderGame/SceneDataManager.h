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
	static GameObject* LoadGameObjects(Scene* scene, const std::vector<std::string>& contentVector);
	static GameObject* LoadGameObjectsClone(Scene* scene, const std::vector<std::string>& contentVector);
	static Scene* Load(std::string path);
	static Scene* LoadRecord();
	static Scene* Reload();
	static void Save();
	static void SaveRecord();


private:
	static std::string _path;
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;

	static Scene* LoadScene(const std::string& path);
};

