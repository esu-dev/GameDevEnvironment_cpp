#pragma once

#include "GameEngine.h"
#include "SampleMonoBehaviour.h"

class DebugPrefab
{
public:
	static GameObject* GetSampleGameObject()
	{
		AudioClip* audioClip = new AudioClip("./Resources/Sound/SE/RetroWeaponLaser03.wav");

		GameObject* gameObject = new GameObject();
		gameObject->AddComponent<AudioSource>()->SetAudioClip(audioClip);
		gameObject->AddComponent<SampleMonoBehaviour>();

		return gameObject;
	}
};