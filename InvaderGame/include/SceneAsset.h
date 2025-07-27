#pragma once

#include "Asset.h"

template <typename T>
class SceneAsset : public Asset<SceneAsset<T>>
{
public:
	static void load_impl() { T::load_scene_asset_impl(); }
};