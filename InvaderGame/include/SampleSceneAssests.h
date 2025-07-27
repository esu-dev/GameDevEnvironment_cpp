#pragma once

#include "SceneAsset.h"

class SampleSceneAssets : public SceneAsset<SampleSceneAssets>
{
public:
	static void load_scene_asset_impl();
};