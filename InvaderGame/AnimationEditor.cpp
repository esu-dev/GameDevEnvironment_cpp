#include "AnimationEditor.h"

#include "imgui_impl_dx11.h"
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
		static int listNum = 0;

		// ImGui表示
		ImGui::SetNextWindowSize(ImVec2(200, 400));
		ImGui::Begin("Animation Editor");

		// 最終的にはシリアライズデータから生成するようにする
		if (ImGui::InputInt("List Num", &listNum))
		{
			for (int i = 0; i < listNum; i++)
			{
				
			}
		}
		ImGui::End();
	}
}