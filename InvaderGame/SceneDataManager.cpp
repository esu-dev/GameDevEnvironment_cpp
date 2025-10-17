#include "SceneDataManager.h"

#include "Utility.h"
#include "GameEngine.h"

std::unordered_map<std::string, Object*>& SceneDataManager::GetInstanceID2PointerMap()
{
	return SceneDataManager::instanceID2PointerMap;
}

Scene* SceneDataManager::Load(std::string path)
{
	// yamlの読み込み
	std::vector<std::string> contentVector;
	FileManager::Read(contentVector, path);


	// シーンの作成
	Scene* scene = SceneManager::CreateScene("YamlTestScene");


	struct InstanceData
	{
		Object* object = nullptr;
		std::vector<std::string> yamlVector;
	};
	std::vector<InstanceData*> instanceDataVector;

	// デシリアライズ
	bool isPacking = false;
	std::string instanceID;
	for (std::string content : contentVector)
	{
		std::smatch m;

		// instanceID
		std::regex re(R"((-{3})\s(\w+))");
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
				instanceDataVector.back()->yamlVector.push_back(m[1].str());
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
			InstanceData* instanceData = new InstanceData();
			instanceData->object = object;

			instanceDataVector.push_back(instanceData);
		}
	}

	// 保持しておいたyamlを元にデシリアライズ
	for (InstanceData* instanceData : instanceDataVector)
	{
		instanceData->object->Deserialize(instanceData->yamlVector);

		// GameObjectをSceneに追加
		/*if (GameObject* gameObject = dynamic_cast<GameObject*>(instanceData->object))
		{
			scene->AddGameObject(gameObject);
		}*/
	}
	//instanceDataVector[0]->object->Deserialize(instanceDataVector[0]->yamlVector);
	if (GameObject* gameObject = dynamic_cast<GameObject*>(instanceDataVector[0]->object))
	{
		scene->AddGameObject(gameObject);
	}

	// instanceIDの値は、全体から検索してポインタを代入

	// オブジェクトのデータベースは忘れずに開放しておく

	//FileManager::Write("Resources/write_test.txt", "aiueo");

	
	instanceDataVector[0]->object->Serialize();
	SceneManager::LoadScene("YamlTestScene");
	Save("write_test");
	return scene;
}

void SceneDataManager::Save(std::string name)
{
	// シーン内の全てのゲームオブジェクトに対してシリアライズを実行
	// 最終的な文字列を書き込む
	std::string serializedData = "";
	for (GameObject* gameObject : SceneManager::GetActiveScene()->GetGameObjectVector())
	{
		// GameObject
		serializedData += "--- " + gameObject->instanceID + "\n";
		serializedData += gameObject->GetName() + ":\n";
		for (std::string line : gameObject->Serialize())
		{
			serializedData += "  " + line + "\n";
		}

		// Component
		for (auto componentPtr : gameObject->GetComponentVector())
		{
			Component* component = componentPtr.get();
			serializedData += "--- " + component->instanceID + "\n";
			serializedData += component->GetName() + ":\n";
			for (std::string line : component->Serialize())
			{
				serializedData += "  " + line + "\n";
			}
		}
	}
	FileManager::Write("Resources/write_test.txt", serializedData);
}


std::unordered_map<std::string, Object*> SceneDataManager::instanceID2PointerMap;