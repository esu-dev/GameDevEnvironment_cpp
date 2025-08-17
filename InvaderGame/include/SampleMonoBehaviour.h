#pragma once

#include "MonoBehaviour.h"

class SampleMonoBehaviour : public MonoBehaviour
{
public:
	SampleMonoBehaviour();
	void OnCollisionEnter2D(Collision2D* collision) override;
	void Start() override;
	void Update() override;

private:
	GameObject* _test = new GameObject();
	GameObject* fpsText = new GameObject();
};