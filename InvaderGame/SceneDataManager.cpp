#include "SceneDataManager.h"

#include "Utility.h"
#include "GameEngine.h"

Scene* SceneDataManager::Load(std::string path)
{
	// yamlの読み込み
	std::vector<std::string> contentVector;
	FileManager::Read(contentVector, path);


	// シーンの作成
	Scene* scene = SceneManager::CreateScene("YamlTestScene");

	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->tag = "MainCamera";
	scene->AddGameObject(camera);


	struct ObjectData
	{
		Object* object;
		std::string
	};

	// デシリアライズ
	std::string instanceID;
	for (std::string content : contentVector)
	{
		std::smatch m;

		// instanceID
		std::regex re(R"((-{3})\s(\w+))");
		if (std::regex_match(content, m, re))
		{
			instanceID = m[2].str();
		}

		re = { R"((\w+):)" };
		if (std::regex_match(content, m, re))
		{
			std::string typeString = m[1].str();

			// インスタンス生成

			// instanceIDをキーとして、yamlとポインタを保持

			// 全てのコンポーネントを生成

			// 保持しておいたyamlを元にデシリアライズ

			// instanceIDの値は、全体から検索してポインタを代入
		}
	}

	// オブジェクトのデータベースは忘れずに開放しておく

	SceneManager::LoadScene("YamlTestScene");
	return scene;
}

void Save(std::string name)
{

}