#include "DirectX.h"

#include "GameSystem.h"

#include "SampleMonoBehaviour.h"

#include "DirectSound.h"

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

	SceneAssets::CreateSampleSceneAsset();

	Scene* currentScene = SceneManager::CreateScene("test");
	SceneManager::CreateScene("test2");
	SceneManager::SetActiveScene(currentScene);

	// Start����
	SceneManager::LoadScene("SampleScene");
}

void GameSystem::Execute()
{
	_delayedExecutionEvent.Invoke();

	// �C�x���g����
	OnUpdateListener.Invoke();

	Physics2D::Update();

    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	// Update����
	SceneManager::GetActiveScene()->Update();

    D3D.m_swapChain->Present(1, 0);
}
