#include "LevelEditor.h"

#include "Input.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "imgui_internal.h"
#include "ImGuiCreator.h"


void LevelEditor::Initialize()
{
	Texture* texture = new Texture();
	texture->Load("Resources/Texture/Frame1.png");
	_frameObject = GameObject::Create();
	_frameObject->AddComponent<SpriteRenderer>()->SetTexture(texture);
}

void LevelEditor::Update()
{
	static bool _isPuttingMode;

	// 設置モード切り替え
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('O'))
	{
		_isPuttingMode = !_isPuttingMode;
	}

	// PuttingMode
	if (_isPuttingMode)
	{
		const float FLASH_INTERVAL = 0.5f;
		static float flashTimer = 0;
		static GameObject* puttingPrefab;

		// Prefabの選択
		if (ImGui::Begin("Prefab Select Window"))
		{
			ImGuiCreator::CreateAssetGui(AssetManager::GetAssetFolder(), [](const std::string& assetName, AssetManager::AssetFile* assetFile) -> void {
				if (GameObject* gameObject = dynamic_cast<GameObject*>(assetFile->object))
				{
					// Prefabの指定
					puttingPrefab = gameObject;
				}
				});
		}
		ImGui::End();

		// 選択中のPrefabを描画
		if (puttingPrefab == nullptr)
		{
			return;
		}
		puttingPrefab->GetTransform()->position = _frameObject->GetTransform()->position;

		// 点滅処理
		if (flashTimer >= FLASH_INTERVAL)
		{
			SpriteRenderer* spriteRenderer = puttingPrefab->GetComponent<SpriteRenderer>();
			spriteRenderer->enabled = !spriteRenderer->enabled;

			flashTimer = 0;
		}
		flashTimer += EngineTime::GetDeltaTime();

		puttingPrefab->Update();

		// フレームの移動
		if (Input::GetKeyDown('W')) _frameObject->GetTransform()->position.Get().y += 1;
		else if (Input::GetKeyDown('A')) _frameObject->GetTransform()->position.Get().x -= 1;
		else if (Input::GetKeyDown('S')) _frameObject->GetTransform()->position.Get().y -= 1;
		else if (Input::GetKeyDown('D')) _frameObject->GetTransform()->position.Get().x += 1;

		// 設置
		if (Input::GetKeyDown('J'))
		{
			GameObject* gameObject = Object::Instantiate(puttingPrefab);
			gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
		}

		// フレームの描画
		_frameObject->Update();
	}
}

GameObject* LevelEditor::_frameObject = nullptr;