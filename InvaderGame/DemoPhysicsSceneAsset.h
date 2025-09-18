#pragma once

#include "SceneAsset.h"

class DemoPhysicsSceneAsset : public SceneAsset<DemoPhysicsSceneAsset>
{
public:
	static Scene* load_scene_asset_impl();
};