#include "AnimationEditor.h"

#include "imgui_impl_dx11.h"
#include "ImGuiCreator.h"

#include "Input.h"

void AnimationEditor::Update()
{
	static bool isEditMode = false;
	if (Input::GetKeyDown('Q')) // ç≈èIìIÇ…ÇÕAÇ…Ç∑ÇÈ
	{
		isEditMode = !isEditMode;

		if (isEditMode)
		{
			_animation = new Animation("TestAnimation");
		}
	}

	if (isEditMode)
	{
		// ImGuiï\é¶
		ImGui::SetNextWindowSize(ImVec2(200, 400));
		ImGui::Begin("Animation Editor");

		ImGuiCreator::Create(_animation);

		ImGui::End();
	}
}

Animation* AnimationEditor::_animation = nullptr;