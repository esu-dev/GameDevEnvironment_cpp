#pragma once

#include "Scene.h"
#include "Asset.h"

using namespace SceneManagement;

template <typename T>
class SceneAsset : public Asset<SceneAsset<T>, Scene*>
{
public:
	static Scene* load_impl() { return T::load_scene_asset_impl(); }
};