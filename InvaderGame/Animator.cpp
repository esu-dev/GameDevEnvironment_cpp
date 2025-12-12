#include "Animator.h"

#include <iterator>

#include "Utility.h"
#include "EngineTime.h"
#include "Animation.h"
#include "SpriteRenderer.h"

//void Animator::SetAnimation(const Animation& animation)
//{
//	_animationVector.push_back(animation);
//}

void Animator::Play(std::string animationName)
{
	_playingTime = 0;
	_textureIndex = 0;

	std::vector<std::string> result;
	std::transform(_animationVector.begin(), _animationVector.end(), std::back_inserter(result),
		[](AnimationState animation) { return animation.AnimationClip->GetAnimationName(); });

	auto itr = std::find(result.begin(), result.end(), animationName);
	__int64 index = std::distance(result.begin(), itr);

	_currentAnimation = _animationVector[index];
	//_currentAnimation->Play();
	//Debug::Log(L"%d, %d", _animationVector.size(), index);
}

void Animator::Start()
{
	_spriteRenderer = this->GetComponent<SpriteRenderer>();

	if (_animationVector.size() == 0)
	{
		return;
	}

	_currentAnimation = _animationVector[0];
}

void Animator::Update()
{
	if (_currentAnimation.AnimationClip == nullptr)
	{
		return;
	}

	if (_textureIndex >= _currentAnimation.AnimationClip->GetAnimDataSetVec().size())
	{
		if (_currentAnimation.AnimationClip->_loopTime)
		{
			_playingTime = 0;
			_textureIndex = 0;
		}
		else
		{
			return;
		}
	}

	if (_playingTime >= _currentAnimation.AnimationClip->GetAnimDataSetVec()[_textureIndex].time / _currentAnimation.Time)
	{
		//Debug::Log(L"%d", _textureIndex);
		_spriteRenderer->SetTexture(_currentAnimation.AnimationClip->GetAnimDataSetVec()[_textureIndex].texture);
		_textureIndex++;
	}

	_playingTime += EngineTime::GetDelataTime();
}