#include "AssetManager.h"

#include <iostream>
#include <codecvt>
#include <variant>
#include "Utility.h"
#include "Object.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Debug.h"
#include "Texture.h"
#include "AnimationClip.h"

void AssetManager::Initialize()
{
	// png
	{
		std::function<void(std::string, std::string, AssetFolder*) > createTextureInstance = [&](std::string rootDirectry, std::string folderName, AssetFolder* currentAssetFolder) -> void {
			std::string directry = rootDirectry + "/" + folderName + "/";

			AssetFolder* assetFolder;

			// アセットフォルダーが存在しないなら生成
			if (currentAssetFolder->name2Datamp.find(folderName) == currentAssetFolder->name2Datamp.end())
			{
				assetFolder = new AssetFolder();
				currentAssetFolder->name2Datamp[folderName] = assetFolder;
			}
			else
			{
				assetFolder = std::get<AssetFolder*>(currentAssetFolder->name2Datamp[folderName]);
			}


			// ファイル
			std::vector<std::string> fileNameVector_png = FileManager::GetAllFileName(directry, "png");

			// すべてのファイルについて処理
			for (std::string fileName : fileNameVector_png)
			{
				// すでにインスタンス化されているならスキップ
				if (assetFolder->name2Datamp.find(fileName) != assetFolder->name2Datamp.end())
				{
					continue;
				}


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
					std::string serializedData =
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

				assetFolder->name2Datamp[fileName] = assetFile;
			}


			// ディレクトリ
			std::vector<std::string> folderNameVector = FileManager::GetAllFolderName(directry);

			// 再帰処理
			for (std::string folderName : folderNameVector)
			{
				createTextureInstance(directry, folderName, assetFolder);
			}
		};

		createTextureInstance("Resources", "Texture", &_assetFolder);
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
				InstantiateAsset(directry, fileName, instanceID2PointerMap);
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
	createAssetFolder("Resources", "Animation", "txt", &_assetFolder);
	
	// Prefab
	createAssetFolder("Resources", "Prefab", "prefab", &_assetFolder);


	std::function<void(std::string, std::string, std::string, AssetFolder*) > createAssetFolder_Scene = [&](std::string rootDirectry, std::string folderName, std::string extension, AssetFolder* currentAssetFolder) -> void {
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
				//CreateInstance(directry, fileName);
				AssetFile* assetFile = new AssetFile();
				assetFile->instanceID = fileName;
				assetFile->object = nullptr;
				newAssetFolder->name2Datamp[fileName] = assetFile;
			}
		}


		// ディレクトリ
		std::vector<std::string> folderNameVector = FileManager::GetAllFolderName(directry);

		// 再帰処理
		for (std::string folderName : folderNameVector)
		{
			createAssetFolder_Scene(directry, folderName, extension, newAssetFolder);
		}
		};

	// Scene
	createAssetFolder_Scene("Resources", "Scenes", "scene", &_assetFolder);
}

void AssetManager::UpdateAssetFolder()
{
	// 既存のデータを削除し、再生成
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
	return _assetFolder;
}

// アセットの作成
// 本来このクラスに作るべきではない
void AssetManager::CreateAsset(const std::string& path, Object* object)
{
	std::string  serializedData =
		"--- " + object->instanceID + "\n" +
		object->GetName() + ":\n";
	serializedData += std_extension::StringVector2String(object->Serialize());

	FileManager::Write(path, serializedData);
}

void AssetManager::CreateInstanceDataVector(std::vector<InstanceData>& outInstanceDataVec, std::unordered_map<std::string, Object*>& id2PtrMap, const std::vector<std::string> contentVector)
{
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
				outInstanceDataVec.back().yamlVector.push_back(m[1].str());
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
			instanceData.object = Activator::Instance->CreateInstance(typeString);
			if (instanceData.object == nullptr)
			{
				continue;
			}
			instanceData.object->instanceID = instanceID;
			id2PtrMap[instanceID] = instanceData.object; // こいつを外でやれば良かった。

			outInstanceDataVec.push_back(instanceData);
		}
	}
}

void AssetManager::InstantiateAsset(const std::string& directry, const std::string& fileName, std::unordered_map<std::string, Object*>& id2PtrMap)
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
	AssetFolder* targetAssetFolder = findAssetFolder(directry, &_assetFolder);

	std::string path = directry + fileName;

	// ファイルの中身を読む込む
	std::vector<std::string> contentVector;
	FileManager::Read(contentVector, path);

	std::vector<InstanceData> instanceDataVector;
	CreateInstanceDataVector(instanceDataVector, id2PtrMap, contentVector);

	// アセットファイルの作成
	AssetFile* assetFile = new AssetFile();
	assetFile->instanceID = instanceDataVector[0].object->instanceID;
	assetFile->object = instanceDataVector[0].object;
	targetAssetFolder->name2Datamp[fileName] = assetFile;

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
	outSerializedData += std::string(gameObject->GetName()) + ":\n";
	for (std::string line : gameObject->Serialize())
	{
		outSerializedData += line + "\n";
	}

	// Component
	for (auto componentPtr : gameObject->GetComponentVector())
	{
		Component* component = componentPtr.get();
		outSerializedData += "--- " + component->instanceID + "\n";
		outSerializedData += std::string(component->GetName()) + ":\n";
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
	outSerializedData.push_back(std::string(gameObject->GetName()) + ":");
	std::vector<std::string> serializedData = gameObject->Serialize();
	outSerializedData.insert(outSerializedData.end(), serializedData.begin(), serializedData.end());

	// Component
	for (auto componentPtr : gameObject->GetComponentVector())
	{
		Component* component = componentPtr.get();
		outSerializedData.push_back("--- " + component->instanceID);
		outSerializedData.push_back(std::string(component->GetName()) + ":");
		std::vector<std::string> serializedData = component->Serialize();
		outSerializedData.insert(outSerializedData.end(), serializedData.begin(), serializedData.end());
	}
}

void AssetManager::SerializeGameObjectInChildren(std::vector<std::string>& outSerializedData, GameObject* gameObject)
{
	// 再帰的にシリアライズ
	std::function<void(GameObject*)> serializeGameObject_Recursively = [&](GameObject* targetGameObject) -> void {
		SerializeGameObject(outSerializedData, targetGameObject);

		// 子要素のシリアライズ
		for (auto transform : targetGameObject->GetTransform()->GetChildVector())
		{
			serializeGameObject_Recursively(transform->gameObject);
		}
		};

	serializeGameObject_Recursively(gameObject);
}

std::unordered_map<std::string, Object*> AssetManager::instanceID2PointerMap;
AssetManager::AssetFolder AssetManager::_assetFolder;
