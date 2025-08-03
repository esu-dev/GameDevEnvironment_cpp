#pragma once

#include "GameObject.h"
#include "Asset.h"

template <typename T>
class PrefabAsset : public Asset<PrefabAsset<T>, GameObject*>
{
public:
	static GameObject* load_impl() { return T::LoadPrefabAsset(); }
};