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
	class AnimationState : public SerializedClass
	{
	public:
		float Time = 1;
		std::string Name = "NewState";
		AnimationClip* AnimationClip = nullptr;

		SERIALIZE3(SerializedClass,
			SERIALIZE_FIELD3(Time),
			SERIALIZE_FIELD3(Name),
			SERIALIZE_FIELD3(AnimationClip)
		)
	};

	int _textureIndex = 0;
	float _playingTime = 0;
	SpriteRenderer* _spriteRenderer;
	AnimationState _currentAnimation;
	std::vector<AnimationState> _animationVector;
};