#include "Animator.h"

void Animator::SetAnimation(Animation* animation)
{
	_animationVector.push_back(animation);
}

void Animator::Play(std::string animationName)
{
	// selectやらを実装する必要がありそうだ。
	// stlのtransformとかで実装できそう。
	//std::find(_animationVector.begin(), _animationVector.end(), animationName)
}