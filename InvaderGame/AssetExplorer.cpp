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

	std::function<void(AssetManager::AssetFolder)> createAssetGui = [&](const AssetManager::AssetFolder& assetFolder) -> void {
		for (auto& pair : assetFolder.name2Datamp)
		{
			std::string assetName = pair.first;
			auto assetData = pair.second;

			// Folder
			if (std::holds_alternative<AssetManager::AssetFolder*>(assetData))
			{
				if (ImGui::TreeNode(assetName.c_str()))
				{
					auto childAssetFolder = *std::get<AssetManager::AssetFolder*>(assetData);
					createAssetGui(childAssetFolder);
					ImGui::TreePop();
				}
			}
			// AssetFile
			else
			{
				auto assetFile = std::get<AssetManager::AssetFile*>(assetData);
				if (ImGui::Selectable((assetName + "  (" + assetFile->instanceID + ")").c_str()))
				{
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
				}
			}
		}
		};

	// windowの生成
	ImGui::Begin("Asset Explorer");

	if (ImGui::Button("Add"))
	{
		// メニュー表示

		// シーンアセットの作成
		AssetCreator::CreateSceneAsset();
	}

	createAssetGui(AssetManager::GetAssetFolder());
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