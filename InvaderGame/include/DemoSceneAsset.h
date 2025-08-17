#pragma once

#include "SceneAsset.h"

class DemoSceneAsset : public SceneAsset<DemoSceneAsset>
{
public:
	static Scene* load_scene_asset_impl();
};