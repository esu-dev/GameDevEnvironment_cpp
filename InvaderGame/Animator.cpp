#include "Animator.h"

void Animator::SetAnimation(Animation* animation)
{
	_animationVector.push_back(animation);
}

void Animator::Play(std::string animationName)
{
	// select������������K�v�����肻�����B
	// stl��transform�Ƃ��Ŏ����ł������B
	//std::find(_animationVector.begin(), _animationVector.end(), animationName)
}