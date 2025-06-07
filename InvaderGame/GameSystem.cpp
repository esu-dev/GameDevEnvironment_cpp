#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "SampleSceneAssests.h"

using namespace SceneManagement;


void GameSystem::AddDelayedExecution(std::function<void()> func)
{
	_delayedExecutionEvent.AddListener(func);
}

void GameSystem::Initialize()
{
	// static class�̏�����
	Time::Initialize();
	Physics2D::Initialize();

	D3D.ChangeMode_2D();

	SampleSceneAssets::CreateSampleSceneAsset();
}

void GameSystem::Execute()
{
	// �C�x���g����
	_delayedExecutionEvent.Invoke();
	OnUpdateListener.Invoke();

	Physics2D::Update();

	// �w�i�F�̐ݒ�
    //float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
	float color[4] = { 0, 0, 0, 1.0 };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	// Update����
	Scene* activeScene;
	if ((activeScene = SceneManager::GetActiveScene()) != nullptr)
	{
		activeScene->Update();
	}

    D3D.m_swapChain->Present(1, 0);
}
