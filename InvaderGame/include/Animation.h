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
		SERIALIZE_FIELD3(_animationName)
	)

	Animation();

	std::string GetAnimationName();

	void Play();

	void Update();


private:
	int _textureIndex = 0;
	float _playingTime = 0;
	bool _isPlaying = false;
	std::string _animationName = "";
	SpriteRenderer* _spriteRenderer = nullptr;
};