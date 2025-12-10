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
	static Scene* Load(std::string path);
	static Scene* Reload();
	static void Save();

private:
	static std::string _path;
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};

