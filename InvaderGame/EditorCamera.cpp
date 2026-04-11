#include "EditorCamera.h"

#include "Input.h"
#include "GameSystem.h"
#include "Camera.h"
#include "Transform.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"

using namespace DirectX;

float EditorCamera::GetSize()
{
	return _size;
}

Vector3 EditorCamera::GetPosition()
{
	return _position;
}

void EditorCamera::GetViewMatrix(float outMat[16])
{
	XMVECTOR cameraPos = _position.ToXMVECTOR();
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(cameraPos, DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorSet(0, 0, 1, 0)), ((Vector3)Vector3::up).ToXMVECTOR());
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(outMat), viewMat);
}

void EditorCamera::GetProjMatrix(float outMat[16])
{
	XMVECTOR cameraPos = _position.ToXMVECTOR();
	DirectX::XMMATRIX projMat = DirectX::XMMatrixOrthographicLH(GameSystem::WINDOW_WIDTH / Camera::Magnification / _size, GameSystem::WINDOW_HEIGHT / Camera::Magnification / _size, 0.0f, 1000.0f);
	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(outMat), projMat);
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
			_position = savedCameraPos;
		}
		else
		{
			savedCameraPos = _position;
			_position = Camera::GetMain()->GetTransform()->position;
		}
	}

	// カメラ起動中
	if (!isEditorCameraOn)
	{
		_position = Camera::GetMain()->GetTransform()->position;
		return;
	}


	// マウスによる移動 (中央クリックドラッグで移動)
	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		ImVec2 delta = ImGui::GetIO().MouseDelta;
		_position += -Vector3::right * delta.x * moveSpeed * 0.01f;
		_position += Vector3::up * delta.y * moveSpeed * 0.01f;
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
Vector3 EditorCamera::_position;
Vector3 EditorCamera::savedCameraPos = Vector3(0, 0, -10);