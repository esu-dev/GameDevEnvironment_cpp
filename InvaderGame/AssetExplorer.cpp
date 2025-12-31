#include "AssetExplorer.h"

#include "framework.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "AssetCreator.h"
#include "imgui_impl_dx11.h"
#include "ImGuiCreator.h"
#include "Object.h"

using namespace SceneManagement;

void AssetExplorer::Update()
{
	static Object* selectedObject = nullptr;

	// windowの生成
	ImGui::Begin("Asset Explorer");

	if (ImGui::Button("Add"))
	{
		// メニュー表示

		// シーンアセットの作成
		AssetCreator::CreateSceneAsset();
	}

	ImGuiCreator::CreateAssetGui(AssetManager::GetAssetFolder(), [](const std::string& assetName, AssetManager::AssetFile* assetFile) -> void {
			// 拡張子の取得
			std::string extension;
			std::smatch smatch;
			if (std::regex_match(assetName, smatch, std::regex(R"(.+\.(\w+))")))
			{
				extension = smatch[1].str();
			}

			// Scene
			if (extension == "scene")
			{
				Debug::Log("エディターで%sを読み込みます。", assetName.c_str());
				SceneDataManager::Load("Resources/Scenes/" + assetName); // これは良くない
			}


			selectedObject = assetFile->object;
		});
	ImGui::End();


	if (selectedObject == nullptr)
	{
		return;
	}

	// Asset Inspectorウィンドウの生成
	if (SerializedClass* serializedObject = dynamic_cast<SerializedClass*>(selectedObject))
	{
		ImGui::Begin("Asset Inspector");
		ImGuiCreator::Create(serializedObject);
		ImGui::End();
	}
}