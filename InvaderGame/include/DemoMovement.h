#pragma once

#include "MonoBehaviour.h"

class DemoMovement : public MonoBehaviour
{
public:
	void Update() override;

private:
	const float SPEED = 5;
};