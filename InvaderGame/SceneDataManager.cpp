#include "SceneDataManager.h"

#include <filesystem>
#include "Utility.h"
#include "GameEngine.h"


Object* SceneDataManager::GetInstance(const std::string& instanceID)
{
	if (instanceID2PointerMap.find(instanceID) != instanceID2PointerMap.end())
	{
		return instanceID2PointerMap[instanceID];
	}

	Debug::Log("存在しないインスタンスを取得しようとしています。[SceneDataManager::GetInstance()]");

	return nullptr;
}

std::unordered_map<std::string, Object*>& SceneDataManager::GetInstanceID2PointerMap()
{
	return SceneDataManager::instanceID2PointerMap;
}

GameObject* SceneDataManager::LoadGameObjects(Scene* scene, const std::vector<std::string>& yamlVector)
{
	std::vector<AssetManager::InstanceData> instanceDataVector;

	// デシリアライズ
	AssetManager::CreateInstanceDataVector(instanceDataVector, instanceID2PointerMap, yamlVector);

	// 保持しておいたyamlを元にデシリアライズ
	GameObject* returnedGameObject = nullptr;
	for (auto& instanceData : instanceDataVector)
	{
		if (instanceData.object == nullptr) continue;

		instanceData.object->Deserialize(instanceData.yamlVector);

		// GameObjectをSceneに追加
		if (GameObject* gameObject = dynamic_cast<GameObject*>(instanceData.object))
		{
			returnedGameObject = gameObject;

			scene->AddGameObject(gameObject);
		}
	}

	return returnedGameObject;
}

GameObject* SceneDataManager::LoadGameObjectsClone(Scene* scene, const std::vector<std::string>& yamlVector)
{
	std::vector<AssetManager::InstanceData> instanceDataVector;
	std::unordered_map<std::string, std::string> originalID2newIDmap;

	// デシリアライズ
	bool isPacking = false;
	std::string originalInstanceID;
	for (std::string content : yamlVector)
	{
		std::smatch m;
		std::regex re(R"((-{3})\s(.+))");
		if (std::regex_match(content, m, re))
		{
			isPacking = false;

			originalInstanceID = m[2].str();
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

			// インスタンス生成
			Object* object = Activator::CreateInstance(typeString);
			object->original = instanceID2PointerMap[originalInstanceID];
			originalID2newIDmap[originalInstanceID] = object->instanceID;
			instanceID2PointerMap[object->instanceID] = object;

			// instanceIDをキーとして、yamlとポインタを保持
			AssetManager::InstanceData instanceData;
			instanceData.object = object;

			instanceDataVector.push_back(instanceData);
		}
	}


	// 保持しておいたyamlを元にデシリアライズ
	GameObject* returnedGameObject = nullptr;
	for (auto& instanceData : instanceDataVector)
	{
		// instanceIDの書き換え
		for (std::string& line : instanceData.yamlVector)
		{
			std::smatch smatch;
			if (std::regex_match(line, smatch, std::regex(R"((-{3} )(.+))")) ||
				std::regex_match(line, smatch, std::regex(R"((.+\(\instanceID\))(.+))")))
			{
				for (auto& pair : originalID2newIDmap)
				{
					if (pair.first == smatch[2].str())
					{
						line = smatch[1].str() + pair.second;
						break;
					}
				}
			}
		}

		instanceData.object->Deserialize(instanceData.yamlVector);

		// GameObjectをSceneに追加
		if (GameObject* gameObject = dynamic_cast<GameObject*>(instanceData.object))
		{
			if (returnedGameObject == nullptr)
			{
				returnedGameObject = gameObject;
			}

			scene->AddGameObject(gameObject);
		}
	}

	return returnedGameObject;
}

Scene* SceneDataManager::Load(std::string path)
{
	_path = path;
	return LoadScene(path);
}

Scene* SceneDataManager::LoadRecord()
{
	// 拡張子を外す
	std::string fileName = FileManager::RemoveExtension(_path);

	return LoadScene(fileName + "_record.txt");
}

Scene* SceneDataManager::Reload()
{
	if (_path == "") return nullptr;
	return Load(_path);
}

void SceneDataManager::Save()
{
	std::string sceneName = SceneManager::GetActiveScene()->GetName();

	// シーン内の全てのゲームオブジェクトに対してシリアライズを実行
	// 最終的な文字列を書き込む
	std::string serializedData = sceneName + "\n";
	for (GameObject* gameObject : SceneManager::GetActiveScene()->GetGameObjectVector())
	{
		AssetManager::SerializeGameObject(serializedData, gameObject);
	}

	// ファイル名を書き換える
	std::filesystem::path path = _path;
	path.replace_filename(sceneName + ".scene");

	FileManager::Write(path, serializedData);
}

void SceneDataManager::SaveRecord()
{
	std::string sceneName = SceneManager::GetActiveScene()->GetName();

	// シーン内の全てのゲームオブジェクトに対してシリアライズを実行
	// 最終的な文字列を書き込む
	std::string serializedData = sceneName + "\n";
	for (GameObject* gameObject : SceneManager::GetActiveScene()->GetGameObjectVector())
	{
		AssetManager::SerializeGameObject(serializedData, gameObject);
	}

	// 拡張子を外す
	std::string fileName = FileManager::RemoveExtension(_path);

	std::string newPath = fileName + "_record.txt";
	FileManager::Write(newPath, serializedData);


	// EngineTimeの保存
	std::string content = std::to_string(EngineTime::GetTotalTime());
	FileManager::Write(fileName + "_time.txt", content);
}

std::string SceneDataManager::_path = "";
std::unordered_map<std::string, Object*> SceneDataManager::instanceID2PointerMap;

Scene* SceneDataManager::LoadScene(const std::string& path)
{
	instanceID2PointerMap.clear();
	RecordManager::RecordVector.clear(); // GameObjectの途中破棄に対応できないからほんとはダメ


	// yamlの読み込み
	std::vector<std::string> yamlVector;
	FileManager::Read(yamlVector, path);


	// シーンの作成
	Scene* scene = new Scene(yamlVector[0]);

	LoadGameObjects(scene, yamlVector);


	SceneManager::SetActiveScene(scene);
	return scene;
}