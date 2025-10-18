#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "SampleSceneAssests.h"

#include "imgui.h"

using namespace SceneManagement;


void GameSystem::AddDelayedExecution(std::function<void()> func)
{
	_delayedExecutionEvent.AddListener(func);
}

void GameSystem::Initialize()
{
	// static class‚Ì‰Šú‰»
	EngineTime::Initialize();
	TimeController::Initialize();
	Physics2D::Initialize();
	Activator::Initialize();

	D3D.ChangeMode_2D();
}

void GameSystem::Execute()
{
	// ƒCƒxƒ“ƒgˆ—
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	Physics2D::Update();
	InputSystem::Update();

	// ”wŒiF‚ÌÝ’è
    //float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	float color[4] = { 0, 0, 0, 1.0 };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	// Updateˆ—
	Scene* activeScene = SceneManager::GetActiveScene();
	if (activeScene != nullptr)
	{
		activeScene->Start();
		activeScene->Update();
	}

	ImGui::CreateContext();

    D3D.m_swapChain->Present(1, 0);
}
