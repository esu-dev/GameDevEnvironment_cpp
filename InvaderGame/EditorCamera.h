#pragma once

class Vector3;

class EditorCamera
{
public:
	static Vector3 GetPosition();
	static void Start();
	static void Update();

private:
	static Vector3 _editorCameraPos;
	static Vector3 savedCameraPos;
};