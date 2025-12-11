#include "SceneDataManager.h"

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

GameObject* SceneDataManager::LoadGameObject(Scene* scene, const std::vector<std::string>& yamlVector)
{
	std::vector<InstanceData> instanceDataVector;

	// デシリアライズ
	bool isPacking = false;
	std::string instanceID;
	for (std::string content : yamlVector)
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

			// インスタンス生成
			Object* object = Activator::CreateInstance(typeString);
			object->instanceID = instanceID;
			instanceID2PointerMap[instanceID] = object;

			// instanceIDをキーとして、yamlとポインタを保持
			InstanceData instanceData = InstanceData();
			instanceData.object = object;

			instanceDataVector.push_back(instanceData);
		}
	}

	// 保持しておいたyamlを元にデシリアライズ
	GameObject* returnedGameObject = nullptr;
	for (const InstanceData& instanceData : instanceDataVector)
	{
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

GameObject* SceneDataManager::LoadGameObjectClone(Scene* scene, const std::vector<std::string>& yamlVector)
{
	std::vector<InstanceData> instanceDataVector;
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
			InstanceData instanceData = InstanceData();
			instanceData.object = object;

			instanceDataVector.push_back(instanceData);
		}
	}


	// 保持しておいたyamlを元にデシリアライズ
	GameObject* returnedGameObject = nullptr;
	for (InstanceData& instanceData : instanceDataVector)
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
			returnedGameObject = gameObject;

			scene->AddGameObject(gameObject);
		}
	}

	return returnedGameObject;
}

Scene* SceneDataManager::Load(std::string path)
{
	_path = path;
	instanceID2PointerMap.clear();
	RecordManager::RecordVector.clear(); // GameObjectの途中破棄に対応できないからほんとはダメ


	// yamlの読み込み
	std::vector<std::string> yamlVector;
	FileManager::Read(yamlVector, path);


	// シーンの作成
	Scene* scene = new Scene(yamlVector[0]);

	LoadGameObject(scene, yamlVector);
	
	
	SceneManager::SetActiveScene(scene);
	return scene;
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
	FileManager::Write("Resources/" + sceneName + ".txt", serializedData);
}


std::string SceneDataManager::_path = "";
std::unordered_map<std::string, Object*> SceneDataManager::instanceID2PointerMap;

void SceneDataManager::DeserializeObject(Scene* scene, const std::vector<InstanceData>& instanceDataVector)
{
	for (const InstanceData& instanceData : instanceDataVector)
	{
		instanceData.object->Deserialize(instanceData.yamlVector);

		// GameObjectをSceneに追加
		if (GameObject* gameObject = dynamic_cast<GameObject*>(instanceData.object))
		{
			scene->AddGameObject(gameObject);
		}
	}
}