#include "AssetCreator.h"

#include "AssetManager.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace SceneManagement;

void AssetCreator::CreateSceneAsset()
{
	// SceneAssetの作成処理をここに実装
	Scene* scene = SceneManager::CreateScene("NewScene");

	std::string sceneName = scene->GetName();

	// シーン内の全てのゲームオブジェクトに対してシリアライズを実行
	// 最終的な文字列を書き込む
	std::string serializedData = sceneName + "\n";
	for (GameObject* gameObject : SceneManager::GetActiveScene()->GetGameObjectVector())
	{
		AssetManager::SerializeGameObject(serializedData, gameObject);
	}
	FileManager::Write("Resources/Scenes/" + sceneName + ".txt", serializedData);
}