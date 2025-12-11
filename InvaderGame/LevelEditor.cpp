#include "LevelEditor.h"

#include "Input.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "imgui_internal.h"


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
			static int selectedIndex = 0;

			int i = 0;
			for (auto& pair : AssetManager::GetInstanceID2PointerMap())
			{
				GameObject* gameObject = dynamic_cast<GameObject*>(pair.second);
				if (gameObject == nullptr)
				{
					continue;
				}

				bool selected = (i == selectedIndex ? true : false);
				if (ImGui::Selectable(pair.second->name.c_str(), &selected))
				{
					selectedIndex = i;

					// Prefabの指定
					puttingPrefab = gameObject;
				}

				i++;
			}
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
		flashTimer += EngineTime::GetDelataTime();

		puttingPrefab->Update();

		// フレームの移動
		if (Input::GetKeyDown('W')) _frameObject->GetTransform()->position.Get().y += 1;
		else if (Input::GetKeyDown('A')) _frameObject->GetTransform()->position.Get().x -= 1;
		else if (Input::GetKeyDown('S')) _frameObject->GetTransform()->position.Get().y -= 1;
		else if (Input::GetKeyDown('D')) _frameObject->GetTransform()->position.Get().x += 1;

		// 設置
		if (Input::GetKeyDown('J'))
		{
			/*GameObject* gameObject = GameObject::Create();
			gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
			gameObject->AddComponent<SpriteRenderer>();*/

			GameObject* gameObject = Object::Instantiate(puttingPrefab);
			gameObject->GetTransform()->position = _frameObject->GetTransform()->position;
		}

		// フレームの描画
		_frameObject->Update();
	}
}

GameObject* LevelEditor::_frameObject = nullptr;