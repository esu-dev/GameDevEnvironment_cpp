#include "AnimationClip.h"

AnimationClip::AnimationClip() {}
AnimationClip::AnimationClip(std::string clipName) : _animationName(clipName) {}

std::string AnimationClip::GetAnimationName()
{
	return _animationName;
}

const std::vector<AnimationClip::AnimationDataSet>& AnimationClip::GetAnimDataSetVec()
{
	return _animationDataSetVector;
}

void AnimationClip::SetLoopTime(bool loopTime)
{
	_loopTime = loopTime;
}

void AnimationClip::SetAnimation(Texture* texture, float time)
{
	AnimationDataSet animationDataSet = AnimationDataSet();
	animationDataSet.texture = texture;
	animationDataSet.time = time;
	_animationDataSetVector.push_back(animationDataSet);
}