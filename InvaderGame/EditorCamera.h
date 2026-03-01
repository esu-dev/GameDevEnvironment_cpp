#pragma once

class Vector3;

class EditorCamera
{
public:
	static float GetSize();
	static Vector3 GetPosition();
	static void GetViewMatrix(float outMat[16]);
	static void GetProjMatrix(float outMat[16]);
	static void Start();
	static void Update();

private:
	static float _size;
	static Vector3 _position;
	static Vector3 savedCameraPos;
};