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

		static std::string instanceID = "Select Animation";
		std::string serializedData = "";
		if (ImGuiCreator::PutPointerField(serializedData, "", "", instanceID))
		{
			// instanceID‚Ì’Šo
			std::smatch smatch;
			if (std::regex_match(serializedData, smatch, std::regex(R"(\(\w+\)(.+))")))
			{
				instanceID = smatch[1].str();
			}
		}
		if (ImGui::Button("Load Animation"))
		{
			_animationClip = (AnimationClip*)AssetManager::GetInstance(instanceID);
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