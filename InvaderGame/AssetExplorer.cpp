#include "AssetExplorer.h"

#include "framework.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "imgui_impl_dx11.h"
#include "Object.h"

using namespace SceneManagement;

void AssetExplorer::Update()
{
	std::function<void(AssetManager::AssetFolder)> createAssetGui = [&](const AssetManager::AssetFolder& assetFolder) -> void {
		for (auto& pair : assetFolder.name2Datamp)
		{
			std::string folderName = pair.first;
			auto assetData = pair.second;

			// Folder
			if (std::holds_alternative<AssetManager::AssetFolder*>(assetData))
			{
				if (ImGui::TreeNode(folderName.c_str()))
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
				if (ImGui::Selectable((assetFile->object->name + "  (" + assetFile->instanceID + ")").c_str()))
				{

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
		SceneManager::CreateScene("NewScene");
	}

	createAssetGui(AssetManager::GetAssetFolder());
	ImGui::End();
}