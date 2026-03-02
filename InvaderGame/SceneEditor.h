#pragma once

#include "Editor.h"

class GameObject;

class SceneEditor : public Editor
{
public:
	static int FieldID;
	static bool GetIsEditMode();
	static void Initialize();
	static void Update();

private:
	static bool _isEditMode;
	static GameObject* _focusFrame;
	static int _gizmoOperation;
};

