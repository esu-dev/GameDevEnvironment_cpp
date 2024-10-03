#pragma once

#include "framework.h"
#include "GameEngine.h"

class Animator : public Component 
{
public:
	void SetAnimation(Animation* animation);

	void Play(std::string animationName);

	void Update() override;

private:
	std::vector<Animation*> _animationVector;
};