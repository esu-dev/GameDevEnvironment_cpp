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
	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('A'))
	{
		isEditMode = !isEditMode;
	}

	if (isEditMode)
	{
		// ImGui•\Ž¦
		ImGui::SetNextWindowSize(ImVec2(400, 400));
		ImGui::Begin("Animation Editor");

		if (ImGui::Button("New Animation"))
		{
			_animationClip = new AnimationClip("TestAnimation");
		}

		/*static char buf[256];
		ImGui::InputText("Load Animation Name", buf, IM_ARRAYSIZE(buf));*/
		//std::string str = "";
		std::string instanceID = "";
		//ImGuiCreator::PutPointerField(str, "", "Select Animation", instanceID);
		if (ImGui::Button("Load Animation"))
		{
			_animationClip = (AnimationClip*)AssetManager::GetInstanceID2PointerMap()[instanceID];
		}

		
		if (_animationClip != nullptr)
		{
			ImGuiCreator::Create(_animationClip);
			if (ImGui::Button("Create"))
			{
				std::string path = "Resources/Animation/" + _animationClip->GetAnimationName() + ".txt";
				AssetManager::CreateAsset(path, _animationClip);

				// window‚ð•Â‚¶‚é
				isEditMode = false;
			}
		}

		ImGui::End();
	}
}

AnimationClip* AnimationEditor::_animationClip = nullptr;