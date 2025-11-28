#include "AnimationEditor.h"

#include "imgui_impl_dx11.h"
#include "ImGuiCreator.h"

#include "std_extension.h"
#include "FileManager.h"
#include "AssetManager.h"
#include "Input.h"

void AnimationEditor::Update()
{
	static bool isEditMode = false;
	if (Input::GetKeyDown('Q')) // 最終的にはAにする
	{
		isEditMode = !isEditMode;
	}

	if (isEditMode)
	{
		// ImGui表示
		ImGui::SetNextWindowSize(ImVec2(400, 400));
		ImGui::Begin("Animation Editor");

		if (ImGui::Button("New Animation"))
		{
			_animation = new Animation("TestAnimation");
		}

		static char buf[256];
		ImGui::InputText("Load Animation Name", buf, IM_ARRAYSIZE(buf));
		if (ImGui::Button("Load Animation"))
		{
			// アセットを開く
		}

		
		if (_animation != nullptr)
		{
			ImGuiCreator::Create(_animation);
			if (ImGui::Button("Create"))
			{
				std::string path = "Resources/Animation/" + _animation->GetAnimationName() + ".txt";
				AssetManager::CreateAsset(path, _animation);

				// windowを閉じる
				isEditMode = false;
			}
		}

		ImGui::End();
	}
}

Animation* AnimationEditor::_animation = nullptr;