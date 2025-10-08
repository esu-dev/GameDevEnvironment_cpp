#pragma once

#include "framework.h"
#include "Scene.h"

using namespace SceneManagement;

class SceneLoader
{
public:
	static Scene* Load(std::string path);
};

