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
	void SetPosition(float x, float y);
	void Update() override;
};