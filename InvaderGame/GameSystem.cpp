#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "TimeController.h"
#include "SceneEditor.h"
#include "SampleSceneAssests.h"

#include "ImGuiUtility.h"

using namespace SceneManagement;


void GameSystem::AddDelayedExecution(std::function<void()> func)
{
	_delayedExecutionEvent.AddListener(func);
}

void GameSystem::Initialize()
{
	// static classの初期化
	EngineTime::Initialize();
	TimeController::Initialize();
	Physics2D::Initialize();
	Activator::Initialize();
	SceneEditor::Initialize();

	D3D.ChangeMode_2D();
}

void GameSystem::Execute()
{
	// 背景色の設定
	//float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	float color[4] = { 0, 0, 0, 1.0 };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	ImGuiUtility::BeginFrame();

	// イベント処理
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	Physics2D::Update();
	InputSystem::Update();
	TimeController::Update();
	SceneEditor::Update();

	// Update処理
	Scene* activeScene = SceneManager::GetActiveScene();
	if (activeScene != nullptr)
	{
		activeScene->Start();
		activeScene->Update();
	}

	// ImGui描画
	// これを最後に持ってこないと、オブジェクトの下にGUIが表示されてしまう。
	ImGuiUtility::Render();

    D3D.m_swapChain->Present(1, 0);
}
