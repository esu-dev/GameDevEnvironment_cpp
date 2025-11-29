#pragma once

#include "framework.h"
#include "Component.h"

class Animation;

class Animator : public Component 
{
public:
	void SetAnimation(Animation* animation);
	void Play(std::string animationName);
	void Update() override;

	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_animationVector)
	)


private:
	std::vector<Animation*> _animationVector;
};