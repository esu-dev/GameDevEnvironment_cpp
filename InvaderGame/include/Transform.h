#pragma once

#include "DirectX.h"
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"

class Transform : public Component
{
public:
	Vector3 position = Vector3(0, 0, 0);
	Quaternion rotation = Quaternion();
	Vector3 scale = Vector3(1, 1, 1);


	Transform();
	Vector3 GetUp();
	Vector3 GetForward();
	void SetPosition(float x, float y);
	void SetLocalPosition(Vector3& vector);
	Vector3 GetLocalPosition();
	void SetParent(Transform* parent);
	void Update() override;


private:
	Vector3 _localPosition = Vector3(0, 0, 0);
	Transform* _parent = nullptr;
};