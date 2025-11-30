#pragma once

#include "framework.h"
#include "Component.h"
#include "AnimationClip.h"

class Animator : public Component 
{
public:
	//void SetAnimation(const Animation& animation);
	void Play(std::string animationName);
	void Start() override;
	void Update() override;

	NAME(Animator)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_animationVector)
	)


private:
	int _textureIndex = 0;
	float _playingTime = 0;
	SpriteRenderer* _spriteRenderer;
	AnimationClip* _currentAnimation;
	std::vector<AnimationClip*> _animationVector;
};