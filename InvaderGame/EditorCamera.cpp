#include "EditorCamera.h"

#include "Input.h"
#include "Camera.h"
#include "Transform.h"
#include "imgui_impl_dx11.h"

const Vector3& EditorCamera::GetPosition()
{
	return _editorCameraPos;
}

void EditorCamera::Start()
{
	//editorCameraPos = Camera::get_main()->GetTransform()->position;
}

void EditorCamera::Update()
{
	static bool isEditorCameraOn;
	static float moveSpeed = 7;
	static Vector3 savedCameraPos;

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
			_editorCameraPos = Camera::get_main()->GetTransform()->position;
		}
	}

	// カメラ起動中
	if (!isEditorCameraOn)
	{
		_editorCameraPos = Camera::get_main()->GetTransform()->position;
		return;
	}


	// カメラの移動
	if (Input::GetKey('W'))
	{
		_editorCameraPos += Vector3::up * moveSpeed * EngineTime::GetEngineDeltaTime();
	}
	else if (Input::GetKey('A'))
	{
		_editorCameraPos += -Vector3::right * moveSpeed * EngineTime::GetEngineDeltaTime();
	}
	else if (Input::GetKey('S'))
	{
		_editorCameraPos += -Vector3::up * moveSpeed * EngineTime::GetEngineDeltaTime();
	}
	else if (Input::GetKey('D'))
	{
		_editorCameraPos += Vector3::right * moveSpeed * EngineTime::GetEngineDeltaTime();
	}


	// ウィンドウ
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Editr Camera");
	ImGui::DragFloat("Move Speed", &moveSpeed);
	ImGui::End();
}

Vector3 EditorCamera::_editorCameraPos;