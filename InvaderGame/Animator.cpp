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
		[](AnimationState animation) {
			if (animation.AnimationClip == nullptr) return std::string();
			return animation.AnimationClip->GetAnimationName();
		});

	auto itr = std::find(result.begin(), result.end(), animationName);
	__int64 index = std::distance(result.begin(), itr);

	if (index >= _animationVector.size())
	{
		return;
	}

	_currentAnimation = _animationVector[index];
}

void Animator::Start()
{
	if (_animationVector.size() == 0)
	{
		return;
	}

	_currentAnimation = _animationVector[0];
}

void Animator::Update()
{
	if (_currentAnimation.Get().AnimationClip == nullptr)
	{
		return;
	}

	if (_textureIndex >= _currentAnimation.Get().AnimationClip->GetAnimDataSetVec().size())
	{
		if (_currentAnimation.Get().AnimationClip->_loopTime)
		{
			_playingTime = 0;
			_textureIndex = 0;
		}
		else
		{
			return;
		}
	}

	if (_playingTime >= _currentAnimation.Get().AnimationClip->GetAnimDataSetVec()[_textureIndex].time / _currentAnimation.Get().Time)
	{
		//Debug::Log(L"%d", _textureIndex);
		this->GetComponent<SpriteRenderer>()->SetTexture(_currentAnimation.Get().AnimationClip->GetAnimDataSetVec()[_textureIndex].texture);
		_textureIndex += 1;
	}

	_playingTime += EngineTime::GetDeltaTime();
}