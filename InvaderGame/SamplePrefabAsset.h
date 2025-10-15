#pragma once

#include "GameEngine.h"
#include "PrefabAsset.h"

class SamplePrefabAsset : public PrefabAsset<SamplePrefabAsset>
{
public:
	static GameObject* LoadPrefabAsset()
	{
		GameObject* gameObject = GameObject::Create();
		gameObject->AddComponent<SpriteRenderer>();

		return gameObject;
	}
};