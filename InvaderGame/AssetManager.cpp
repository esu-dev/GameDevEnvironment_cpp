#include "AssetManager.h"

#include <iostream>
#include "framework.h"
#include "Utility.h"
#include "Debug.h"

void AssetManager::Initialize()
{
	// テクスチャアセットの相対パスをすべて取得する

	// 画像の相対パスをすべて取得する


	// テクスチャアセットをすべて読み込む
	FileManager::GetAllPath("Resources/Texture/", "txt");


	// それぞれの画像に対応するアセットが存在するか調べる
	// ない場合はアセットを作成する

	// テクスチャを生成する
}