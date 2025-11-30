#include "Animation.h"

#include "GameEngine.h"

Animation::Animation() {}

std::string Animation::GetAnimationName()
{
	return _animationName;
}

void Animation::Play()
{
	_isPlaying = true;
	_playingTime = 0;
	_textureIndex = 0;
}

// Animation単体で動かすことを想定していないため、Update処理をGameObjectと分離しない。
// Unityではおそらく別でUpdate処理を持っている。
void Animation::Update()
{
	//if (_isPlaying)
	//{
	//	if (_textureIndex >= _animationDataSetVector.size())
	//	{
	//		if (_loopTime)
	//		{
	//			Play();
	//		}
	//		else
	//		{
	//			_isPlaying = false;
	//			return;
	//		}
	//	}

	//	if (_playingTime >= _animationDataSetVector[_textureIndex].time)
	//	{
	//		//Debug::Log(L"%d", _textureIndex);
	//		_spriteRenderer->SetTexture(_animationDataSetVector[_textureIndex].texture);
	//		_textureIndex++;
	//	}

	//	_playingTime += EngineTime::GetDelataTime();
	//}
}