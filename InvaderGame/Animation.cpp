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

// Animation�P�̂œ��������Ƃ�z�肵�Ă��Ȃ����߁AUpdate������GameObject�ƕ������Ȃ��B
// Unity�ł͂����炭�ʂ�Update�����������Ă���B
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