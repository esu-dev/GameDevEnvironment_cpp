#include "SceneLoader.h"

#include "Utility.h"
#include "GameEngine.h"

Scene* SceneLoader::Load(std::string path)
{
	std::vector<std::string> contentVector;
	FileReader::Read(contentVector, path);

	Scene* scene = SceneManager::CreateScene("YamlTestScene");

	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->tag = "MainCamera";
	scene->AddGameObject(camera);

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

			// マクロでインスタンス生成
			//Activator
		}
	}

	SceneManager::LoadScene("YamlTestScene");
	return scene;
}