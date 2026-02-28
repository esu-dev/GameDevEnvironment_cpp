#pragma once

class Vector3;

class EditorCamera
{
public:
	static float GetSize();
	static Vector3 GetPosition();
	static void Start();
	static void Update();

private:
	static float _size;
	static Vector3 _editorCameraPos;
	static Vector3 savedCameraPos;
};