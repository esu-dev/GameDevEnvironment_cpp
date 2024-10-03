#pragma once

#include "framework.h"

class Texture;
class SpriteRenderer;

class Animation
{
public:
	Animation(std::string animationName, SpriteRenderer* spriteRenderer);

	std::string GetAnimationName();

	void SetAnimation(Texture* texture, float time);

	void Play();

	void Update();

private:
	class AnimationDataSet
	{
	public:
		Texture* texture;
		float time;
	};

	int _textureIndex = 0;
	float _playingTime = 0;
	bool _isPlaying = false;
	std::string _animationName;
	SpriteRenderer* _spriteRenderer;
	std::vector<AnimationDataSet*> _animationDataSetVector;
};