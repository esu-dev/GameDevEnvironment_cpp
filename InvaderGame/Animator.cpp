#include "Animator.h"

#include <iterator>

#include "Animation.h"

void Animator::SetAnimation(Animation* animation)
{
	_animationVector.push_back(animation);
}

void Animator::Play(std::string animationName)
{
	std::vector<std::string> result;
	std::transform(_animationVector.begin(), _animationVector.end(), std::back_inserter(result),
		[](Animation* animation) { return animation->GetAnimationName(); });

	auto itr = std::find(result.begin(), result.end(), animationName);
	__int64 index = std::distance(result.begin(), itr);

	_animationVector[index]->Play();
	Debug::Log(L"%d, %d", _animationVector.size(), index);
}

void Animator::Update()
{
	for (Animation* anim : _animationVector)
	{
		anim->Update();
	}
}