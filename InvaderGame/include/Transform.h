#pragma once

#include "framework.h"
#include "DirectX.h"
#include "GameEngine.h"
#include "Record.h"

class Transform : public Component
{
public:
	Record<Vector3> position;
	Quaternion rotation = Quaternion();
	Vector3 scale = Vector3(1, 1, 1);

	SERIALIZE3(Component,
		SERIALIZE_FIELD3(position),
		SERIALIZE_FIELD3(scale)
	)

	Transform();
	
	Vector3 GetUp();
	Vector3 GetForward();
	void SetPosition(float x, float y);
	void SetLocalPosition(Vector3& vector);
	Vector3 GetLocalPosition();
	void SetParent(Transform* parent);
	void Update() override;

	NAME(Transform)


private:
	Vector3 _localPosition = Vector3(0, 0, 0);
	Transform* _parent = nullptr;
};