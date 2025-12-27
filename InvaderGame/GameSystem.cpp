#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SceneEditor.h"

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
	Physics2D::Initialize();
	Activator::Initialize();
	AssetManager::Initialize();
	SceneEditor::Initialize();

	D3D.ChangeMode_2D();
}

void GameSystem::Execute()
{
	// 背景色の設定
	const auto& bg = Camera::backgroundColor;
	float color[4] = { bg.x, bg.y, bg.z, bg.w };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	ImGuiUtility::BeginFrame();

	// イベント処理
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	Input::Update();
	Physics2D::Update();
	InputSystem::Update();
	SceneEditor::Update();

	// Update処理
	Scene* activeScene = SceneManager::GetActiveScene();
	if (activeScene != nullptr)
	{
		activeScene->Start();
		activeScene->Update();
	}

	// 登録されたレンダリング関数を order 順に実行する
	for (RenderingData* rd : _renderingDataVector)
	{
		if (rd && rd->function)
		{
			rd->function();
		}
		delete rd;
	}
	_renderingDataVector.clear();

	// ImGui描画
	// これを最後に持ってこないと、オブジェクトの下にGUIが表示されてしまう。
	ImGuiUtility::Render();

    D3D.m_swapChain->Present(1, 0);
}

void GameSystem::AddRenderingData(int order, std::function<void()> func)
{
	RenderingData* renderData = new RenderingData();
	renderData->order = order;
	renderData->function = func;

	// 挿入位置を order 昇順で検索して挿入する
	auto it = std::find_if(_renderingDataVector.begin(), _renderingDataVector.end(), [&](RenderingData* rd) {
		return rd->order > order;
	});

	if (it != _renderingDataVector.end())
	{
		_renderingDataVector.insert(it, renderData);
	}
	else
	{
		_renderingDataVector.push_back(renderData);
	}
}
