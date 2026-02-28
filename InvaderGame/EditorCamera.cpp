#include "EditorCamera.h"

#include "Input.h"
#include "Camera.h"
#include "Transform.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"

float EditorCamera::GetSize()
{
	return _size;
}

Vector3 EditorCamera::GetPosition()
{
	return _editorCameraPos;
}

void EditorCamera::Start()
{

}

void EditorCamera::Update()
{
	static bool isEditorCameraOn;
	static float moveSpeed = 4;


	if (Input::GetKey(VK_CONTROL) && Input::GetKeyDown('C'))
	{
		isEditorCameraOn = !isEditorCameraOn;
		
		// カメラ起動
		if (isEditorCameraOn)
		{
			_editorCameraPos = savedCameraPos;
		}
		else
		{
			savedCameraPos = _editorCameraPos;
			_editorCameraPos = Camera::GetMain()->GetTransform()->position;
		}
	}

	// カメラ起動中
	if (!isEditorCameraOn)
	{
		_editorCameraPos = Camera::GetMain()->GetTransform()->position;
		return;
	}


	// マウスによる移動 (右クリックドラッグで移動)
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		ImVec2 delta = ImGui::GetIO().MouseDelta;
		_editorCameraPos += -Vector3::right * delta.x * moveSpeed * 0.01f;
		_editorCameraPos += Vector3::up * delta.y * moveSpeed * 0.01f;
	}

	// ズーム
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		float zoomAmount = ImGui::GetIO().MouseWheel * 0.1f;
		if (zoomAmount != 0)
		{
			_size += zoomAmount;
		}
	}


	// ウィンドウ
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Editor Camera");
	ImGui::DragFloat("Move Speed", &moveSpeed);
	ImGui::End();
}

float EditorCamera::_size = 1.0f;
Vector3 EditorCamera::_editorCameraPos;
Vector3 EditorCamera::savedCameraPos = Vector3(0, 0, -10);