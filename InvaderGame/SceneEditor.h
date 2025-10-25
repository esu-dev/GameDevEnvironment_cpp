#pragma once

#include "Editor.h"

class GameObject;

class SceneEditor : public Editor
{
public:
	static void Initialize();
	static void Update();

private:
	static bool _isEditMode;
	static GameObject* _frameObject;
};

