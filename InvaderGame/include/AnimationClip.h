#pragma once

#include "Object.h"
#include "Texture.h"

class AnimationClip : public Object
{
public:
	class AnimationDataSet : public SerializedClass
	{
	public:
		Texture* texture = nullptr;
		float time = 0;

		SERIALIZE3(SerializedClass,
			SERIALIZE_FIELD3(time),
			SERIALIZE_FIELD3(texture)
		)
	};

	bool _loopTime = false;

	NAME(AnimationClip)
	SERIALIZE3(Object,
		SERIALIZE_FIELD3(_animationName),
		SERIALIZE_FIELD3(_loopTime),
		SERIALIZE_FIELD3(_animationDataSetVector)
	)

	AnimationClip();
	AnimationClip(std::string clipName);

	std::string GetAnimationName();
	const std::vector<AnimationDataSet>& GetAnimDataSetVec();

	void SetLoopTime(bool loopTime);
	void SetAnimation(Texture* texture, float time);


private:
	std::string _animationName = "";
	std::vector<AnimationDataSet> _animationDataSetVector;
};