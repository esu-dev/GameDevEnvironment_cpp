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
		[](AnimationClip* animation) { return animation->GetAnimationName(); });

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
	if (_currentAnimation == nullptr)
	{
		return;
	}

	if (_textureIndex >= _currentAnimation->GetAnimDataSetVec().size())
	{
		if (_currentAnimation->_loopTime)
		{
			_playingTime = 0;
			_textureIndex = 0;
		}
		else
		{
			return;
		}
	}

	if (_playingTime >= _currentAnimation->GetAnimDataSetVec()[_textureIndex].time)
	{
		//Debug::Log(L"%d", _textureIndex);
		_spriteRenderer->SetTexture(_currentAnimation->GetAnimDataSetVec()[_textureIndex].texture);
		_textureIndex++;
	}

	_playingTime += EngineTime::GetDelataTime();
}