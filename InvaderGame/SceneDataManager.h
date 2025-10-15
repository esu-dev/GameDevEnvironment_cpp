#pragma once

#include "framework.h"
#include "Scene.h"

class Object;

using namespace SceneManagement;

class SceneDataManager
{
public:
	static std::unordered_map<std::string, Object*>& GetInstanceID2PointerMap();
	static Scene* Load(std::string path);
	static void Save(std::string name);

private:
	static std::unordered_map<std::string, Object*> instanceID2PointerMap;
};

