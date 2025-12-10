#include "AssetManager.h"

#include <iostream>
#include <codecvt>
#include "framework.h"
#include "Utility.h"
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Debug.h"
#include "Texture.h"
#include "AnimationClip.h"

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
			if (std::regex_match(fileName, wsmatch, std::regex(R"((.+)\.\w+)")))
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

	// AnimationClip
	{
		std::string directry = "Resources/Animation/";
		std::vector<std::string> fileNameVector = FileManager::GetAllFileName(directry, "txt");

		for (std::string fileName : fileNameVector)
		{
			std::string path = directry + fileName;

			// アセットがあればインスタンスの生成
			if (FileManager::Exist(path))
			{
				CreateInstance(directry, fileName);
			}
		}
	}

	// Prefab
	{
		std::string directry = "Resources/Prefab/";
		std::vector<std::string> fileNameVector = FileManager::GetAllFileName(directry, "prefab");

		for (std::string fileName : fileNameVector)
		{
			std::string path = directry + fileName;

			// アセットがあればインスタンスの生成
			if (FileManager::Exist(path))
			{
				CreateInstance(directry, fileName);
			}
		}
	}
}

Object* AssetManager::GetInstance(const std::string& instanceID)
{
	if (instanceID2PointerMap.find(instanceID) != instanceID2PointerMap.end())
	{
		return instanceID2PointerMap[instanceID];
	}
	return nullptr;
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

void AssetManager::CreateInstance(const std::string& directry, const std::string& fileName)
{
	std::string path = directry + fileName;

	// ファイルの中身を読む込む
	std::vector<std::string> contentVector;
	FileManager::Read(contentVector, path);


	struct InstanceData
	{
		Object* object = nullptr;
		std::vector<std::string> yamlVector;
	};
	std::vector<InstanceData> instanceDataVector;

	bool isPacking = false;
	std::string instanceID;
	for (std::string content : contentVector)
	{
		std::smatch m;

		// instanceID
		std::regex re(R"((-{3})\s(.+))");
		if (std::regex_match(content, m, re))
		{
			isPacking = false;

			instanceID = m[2].str();
		}


		// まとめる
		if (isPacking)
		{
			if (std::regex_match(content, m, std::regex(R"(\s{2}(.+))")))
			{
				instanceDataVector.back().yamlVector.push_back(m[1].str());
			}
		}


		// Objectの生成
		re = { R"(^(\w+):)" };
		if (std::regex_match(content, m, re))
		{
			isPacking = true;

			std::string typeString = m[1].str();

			InstanceData instanceData = InstanceData();

			// インスタンス生成
			instanceData.object = Activator::CreateInstance(typeString);
			if (instanceData.object == nullptr)
			{
				continue;
			}
			instanceData.object->instanceID = instanceID;
			instanceData.object->name = fileName;
			instanceID2PointerMap[instanceID] = instanceData.object;

			instanceDataVector.push_back(instanceData);
		}
	}

	// 保持しておいたyamlを元にデシリアライズ
	for (InstanceData& instanceData : instanceDataVector)
	{
		instanceData.object->Deserialize(instanceData.yamlVector);
	}
}

void AssetManager::SerializeGameObject(std::string& outSerializedData, GameObject* gameObject)
{
	// GameObject
	outSerializedData += "--- " + gameObject->instanceID + "\n";
	outSerializedData += gameObject->GetName() + ":\n";
	for (std::string line : gameObject->Serialize())
	{
		outSerializedData += line + "\n";
	}

	// Component
	for (auto componentPtr : gameObject->GetComponentVector())
	{
		Component* component = componentPtr.get();
		outSerializedData += "--- " + component->instanceID + "\n";
		outSerializedData += component->GetName() + ":\n";
		for (std::string line : component->Serialize())
		{
			outSerializedData += line + "\n";
		}
	}
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;
