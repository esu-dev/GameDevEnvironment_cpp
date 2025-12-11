#pragma once

class GameObject;

class LevelEditor
{
public:
	static void Initialize();
	static void Update();

private:
	static GameObject* _frameObject;
};