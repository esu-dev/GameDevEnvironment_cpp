#pragma once

#include "Collider2D.h"
#include "Vector2.h"

class BoxCollider2D : public Collider2D
{
public:
	BoxCollider2D();
	~BoxCollider2D();
	void SetSize(Vector2 size);

	void OnEnable() override;
	void OnDisable() override;
	void Start() override;
	void Update() override;


private:
	void SynchronizeBodyWithGameObject();
};
