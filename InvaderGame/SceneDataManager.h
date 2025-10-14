#pragma once

#include "framework.h"
#include "Scene.h"

using namespace SceneManagement;

class SceneDataManager
{
public:
	static Scene* Load(std::string path);
	static void Save(std::string name);
};

