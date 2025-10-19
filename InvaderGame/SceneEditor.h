#pragma once

#include "Editor.h"

class SceneEditor : public Editor
{
public:
	static void Initialize();
	void Update() override;
};

