#pragma once

#include "Scene.h"
#include "SceneAsset.h"

using namespace SceneManagement;

class SampleSceneAssets : public SceneAsset<SampleSceneAssets>
{
public:
	static Scene* load_scene_asset_impl();
};