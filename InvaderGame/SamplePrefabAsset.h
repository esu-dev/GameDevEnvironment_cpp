#pragma once

#include "GameEngine.h"
#include "PrefabAsset.h"

class SamplePrefabAsset : public PrefabAsset<SamplePrefabAsset>
{
public:
	static GameObject* LoadPrefabAsset()
	{
		GameObject* gameObject = new GameObject();
		gameObject->AddComponent<SpriteRenderer>();

		return gameObject;
	}
};