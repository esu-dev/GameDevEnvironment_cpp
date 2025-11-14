#include "AssetManager.h"

#include <iostream>
#include "framework.h"
#include "Utility.h"
#include "Object.h"
#include "Debug.h"
#include "Texture.h"

void AssetManager::Initialize()
{
	// 画像をすべて読み込む
	// とりあえず対応はpngのみ
	std::vector<std::wstring> fileNameVector_png = FileManager::GetAllFileName("Resources/Texture/", "png");

	for (std::wstring fileName : fileNameVector_png)
	{
		// Textureの生成
		std::wstring imgPath = L"Resources/Texture/" + fileName;
		Texture* texture = new Texture(imgPath);

		// 拡張子の変更
		std::wstring textFileName;
		std::wsmatch wsmatch;
		if (std::regex_match(fileName, wsmatch, std::wregex(L"(.+)\\.\\w+")))
		{
			textFileName = wsmatch[1].str() + L".txt";
		}
		else
		{
			Debug::Log(L"拡張子を変更できませんでした．");
			continue;
		}

		// アセットが存在するか確認
		std::wstring path = L"Resources/Texture/" + textFileName;
		
		// あればinstanceIDの設定
		if (FileManager::Exist(path))
		{
			// ファイルの中身を読む込む
			std::vector<std::string> contentVector;
			FileManager::Read(contentVector, path);

			// instanceIDを取得する
			std::string instanceID;
			std::smatch smatch;
			if (std::regex_match(contentVector[0], smatch, std::regex(R"(-{3}\s(.+))")))
			{
				instanceID = smatch[1].str();
			}

			texture->instanceID = instanceID;
		}
		// なければアセットの作成
		else
		{
			std::string  serializedData =
				"--- " + texture->instanceID + "\n" + 
				texture->GetName() + ":\n";
			for (std::string line : texture->Serialize())
			{
				serializedData += line + "\n";
			}

			FileManager::Write(path, serializedData);
		}

		instanceID2PointerMap[texture->instanceID] = texture;
	}
}

std::unordered_map<std::string, Object*>& AssetManager::GetInstanceID2PointerMap()
{
	return AssetManager::instanceID2PointerMap;
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;