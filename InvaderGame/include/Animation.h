#pragma once

#include "framework.h"
#include "SerializedClass.h"
#include "Texture.h"

class SpriteRenderer;

class Animation : public Object
{
public:
	NAME(Animation)
	SERIALIZE3(Object,
		SERIALIZE_FIELD3(_animationName),
		SERIALIZE_FIELD3(_loopTime),
		SERIALIZE_FIELD3(_animationDataSetVector)
	)

	Animation(std::string animationClipName);
	Animation(std::string animationName, SpriteRenderer* spriteRenderer);

	std::string GetAnimationName();

	void SetLoopTime(bool loopTime);
	void SetAnimation(Texture* texture, float time);

	void Play();

	void Update();

private:
	class AnimationDataSet : public SerializedClass
	{
	public:
		Texture* texture;
		float time;

		SERIALIZE3(SerializedClass,
			SERIALIZE_FIELD3(time),
			SERIALIZE_FIELD3(texture)
		)
	};

	int _textureIndex = 0;
	float _playingTime = 0;
	bool _isPlaying = false;
	bool _loopTime = false;
	std::string _animationName;
	SpriteRenderer* _spriteRenderer = nullptr;
	std::vector<AnimationDataSet> _animationDataSetVector;
};