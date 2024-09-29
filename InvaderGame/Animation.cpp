#include "Animation.h"

#include "GameEngine.h"

Animation::Animation(std::string animationName) : _animationName(animationName), _spriteRenderer(nullptr) {}

std::string Animation::GetAnimationName()
{
	return _animationName;
}

void Animation::SetSpriteRenderer(SpriteRenderer* spriteRenderer)
{
	_spriteRenderer = spriteRenderer;
}

void Animation::SetAnimation(Texture* texture, float time)
{
	AnimationDataSet* animationDataSet = new AnimationDataSet();
	animationDataSet->texture = texture;
	animationDataSet->time = time;
	_animationDataSetVector.push_back(animationDataSet);
}

void Animation::Play()
{
	_isPlaying = true;
}

// Animation単体で動かすことを想定していないため、Update処理尾GameObjectと分離しない。
// Unityではおそらく別でUpdate処理を持っている。
void Animation::Update()
{
	if (_isPlaying)
	{
		if (_textureIndex > _animationDataSetVector.size())
		{
			_isPlaying = false;
			return;
		}

		if (_playingTime >= _animationDataSetVector[_textureIndex]->time)
		{
			_spriteRenderer->SetTexture(_animationDataSetVector[_textureIndex]->texture);
			_textureIndex++;
		}

		_playingTime += Time::GetDelataTime();
	}
}