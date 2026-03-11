#include "ImGuiUtility.h"

#include "Direct3D.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void ImGuiUtility::Initialize(HWND hWnd)
{
	ImGuiContext* imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//imGuiContext->IO.DisplaySize = ImVec2(100, 100);
	//ImGui::SetCurrentContext(imGuiContext);
	/*ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();*/
	
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Direct3D::GetInstance()._device.Get(), Direct3D::GetInstance().m_deviceContext.Get());
}

void ImGuiUtility::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiUtility::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}