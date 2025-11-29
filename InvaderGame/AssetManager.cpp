#include "AssetManager.h"

#include <iostream>
#include <codecvt>
#include "framework.h"
#include "Utility.h"
#include "Object.h"
#include "Debug.h"
#include "Texture.h"
#include "Animation.h"

void AssetManager::Initialize()
{
	// png
	{
		std::string directry = "Resources/Texture/";
		std::vector<std::string> fileNameVector_png = FileManager::GetAllFileName(directry, "png");

		for (std::string fileName : fileNameVector_png)
		{
			// Textureの生成
			std::string imgPath = directry + fileName;
			Texture* object = new Texture(imgPath);

			// 拡張子の変更
			std::string textFileName;
			std::smatch wsmatch;
			if (std::regex_match(fileName, wsmatch, std::regex(R"((.+).\w+)")))
			{
				textFileName = wsmatch[1].str() + ".txt";
			}
			else
			{
				Debug::Log(L"拡張子を変更できませんでした．");
				return;
			}

			// アセットが存在するか確認
			std::string path = directry + textFileName;

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

				object->instanceID = instanceID;
			}
			// なければアセットの作成
			else
			{
				std::string  serializedData =
					"--- " + object->instanceID + "\n" +
					object->GetName() + ":\n";
				serializedData += std_extension::StringVector2String(object->Serialize());

				FileManager::Write(path, serializedData);
			}

			// アセット名の設定
			object->name = fileName;

			instanceID2PointerMap[object->instanceID] = object;
		}
	}

	// Animation
	{
		std::string directry = "Resources/Animation/";
		std::vector<std::string> fileNameVector = FileManager::GetAllFileName(directry, "txt");

		for (std::string fileName : fileNameVector)
		{
			std::string path = directry + fileName;

			// あればinstanceIDの設定
			if (FileManager::Exist(path))
			{
				Animation* object = new Animation(fileName);

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

				object->instanceID = instanceID;
				object->name = fileName;

				instanceID2PointerMap[object->instanceID] = object;
			}
		}
	}
}

std::unordered_map<std::string, Object*>& AssetManager::GetInstanceID2PointerMap()
{
	return AssetManager::instanceID2PointerMap;
}

void AssetManager::CreateAsset(const std::string& path, Object* object)
{
	std::string  serializedData =
		"--- " + object->instanceID + "\n" +
		object->GetName() + ":\n";
	serializedData += std_extension::StringVector2String(object->Serialize());

	FileManager::Write(path, serializedData);
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;
