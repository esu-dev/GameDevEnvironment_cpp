#pragma once

#include "EngineFramework.h"

#include "Scene.h"

using namespace SceneManagement;

class YamlUtility
{
public:
	static Scene* fromYaml(std::string yaml);
};

