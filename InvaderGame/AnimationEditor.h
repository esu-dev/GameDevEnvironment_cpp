#pragma once

#include "AnimationClip.h"

class AnimationEditor
{
public:
	static void Update();


private:
	static AnimationClip* _animationClip;
};