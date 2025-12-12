#include "AssetManager.h"

#include <iostream>
#include <codecvt>
#include <variant>
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
		std::function<void(std::string, std::string, AssetFolder*) > createTextureInstance = [&](std::string rootDirectry, std::string folderName, AssetFolder* currentAssetFolder) -> void {
			std::string directry = rootDirectry + "/" + folderName + "/";

			AssetFolder* newAssetFolder = new AssetFolder();
			currentAssetFolder->name2Datamp[folderName] = newAssetFolder;


			// ファイル
			std::vector<std::string> fileNameVector_png = FileManager::GetAllFileName(directry, "png");

			// すべてのファイルについて処理
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

				AssetFile* assetFile = new AssetFile();
				assetFile->instanceID = object->instanceID;
				assetFile->object = object;

				newAssetFolder->name2Datamp[fileName] = assetFile;
			}


			// ディレクトリ
			std::vector<std::string> folderNameVector = FileManager::GetAllFolderName(directry);

			// 再帰処理
			for (std::string folderName : folderNameVector)
			{
				createTextureInstance(directry, folderName, newAssetFolder);
			}
		};

		createTextureInstance("Resources", "Texture", &assetFolder);
	}


	std::function<void(std::string, std::string, std::string, AssetFolder*) > createAssetFolder = [&](std::string rootDirectry, std::string folderName, std::string extension, AssetFolder* currentAssetFolder) -> void {
		std::string directry = rootDirectry + "/" + folderName + "/";

		AssetFolder* newAssetFolder = new AssetFolder();
		currentAssetFolder->name2Datamp[folderName] = newAssetFolder;


		// ファイル
		std::vector<std::string> fileNameVector = FileManager::GetAllFileName(directry, extension);

		for (std::string fileName : fileNameVector)
		{
			std::string path = directry + fileName;

			// アセットがあればインスタンスの生成
			if (FileManager::Exist(path))
			{
				CreateInstance(directry, fileName);
			}
		}


		// ディレクトリ
		std::vector<std::string> folderNameVector = FileManager::GetAllFolderName(directry);

		// 再帰処理
		for (std::string folderName : folderNameVector)
		{
			createAssetFolder(directry, folderName, extension, newAssetFolder);
		}
	};

	// AnimationClip
	createAssetFolder("Resources", "Animation", "txt", &assetFolder);
	
	// Prefab
	createAssetFolder("Resources", "Prefab", "prefab", &assetFolder);
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

const AssetManager::AssetFolder& AssetManager::GetAssetFolder()
{
	return assetFolder;
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
	// ディレクトリからAssetFolderを探索
	std::function<AssetFolder*(std::string, AssetFolder*)> findAssetFolder = [&](const std::string& directry, AssetFolder* currentAssetFolder) -> AssetFolder* {
		std::smatch smatch;
		if (std::regex_match(directry, smatch, std::regex(R"(.*/(.+)(/.*))")))
		{
			auto map = currentAssetFolder->name2Datamp;
			auto nextAssetFolder = std::get<AssetFolder*>(map[smatch[1].str()]);
			
			return findAssetFolder(smatch[2].str(), nextAssetFolder);
		}
		return currentAssetFolder;
	};
	AssetFolder* targetAssetFolder = findAssetFolder(directry, &assetFolder);

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

			AssetFile* assetFile = new AssetFile();
			assetFile->instanceID = instanceID;
			assetFile->object = instanceData.object;

			targetAssetFolder->name2Datamp[fileName] = assetFile;

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

void AssetManager::SerializeGameObject(std::vector<std::string>& outSerializedData, GameObject* gameObject)
{
	// GameObject
	outSerializedData.push_back("--- " + gameObject->instanceID);
	outSerializedData.push_back(gameObject->GetName() + ":");
	std::vector<std::string> serializedData = gameObject->Serialize();
	outSerializedData.insert(outSerializedData.end(), serializedData.begin(), serializedData.end());

	// Component
	for (auto componentPtr : gameObject->GetComponentVector())
	{
		Component* component = componentPtr.get();
		outSerializedData.push_back("--- " + component->instanceID);
		outSerializedData.push_back(component->GetName() + ":");
		std::vector<std::string> serializedData = component->Serialize();
		outSerializedData.insert(outSerializedData.end(), serializedData.begin(), serializedData.end());
	}
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;
AssetManager::AssetFolder AssetManager::assetFolder;
