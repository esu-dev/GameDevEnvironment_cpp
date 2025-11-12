#include "AssetManager.h"

#include <iostream>
#include "framework.h"
#include "Utility.h"
#include "Debug.h"
#include "Texture.h"

void AssetManager::Initialize()
{
	// テクスチャアセットの相対パスをすべて取得する

	// 画像の相対パスをすべて取得する


	// テクスチャアセットをすべて読み込む
	std::vector<std::wstring> pathVector = FileManager::GetAllPath("Resources/Texture/", "txt");


	// それぞれの画像に対応するアセットが存在するか調べる
	// ない場合はアセットを作成する

	// テクスチャを生成する
	for (std::wstring path : pathVector)
	{
		// ファイルの中身を読む込む
		std::vector<std::string> contentVector;
		FileManager::Read(contentVector, L"Resources/Texture/" + path);

		// instanceIDを取得する
		std::string instanceID;
		std::smatch smatch;
		if (std::regex_match(contentVector[0], smatch, std::regex(R"(-{3}\s(.+))")))
		{
			instanceID = smatch[1].str();
		}

		// Textureを生成
		instanceID2PointerMap[instanceID] = new Texture(instanceID, path);
	}
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;