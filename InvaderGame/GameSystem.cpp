#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SceneEditor.h"
#include "GameState.h"

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
	ULONGLONG startTime = GetTickCount64();

	// 背景色の設定
	const auto& bg = Camera::backgroundColor;
	float color[4] = { bg.x, bg.y, bg.z, bg.w };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	Debug::Log("%d", (int)(GetTickCount64() - startTime));

	ImGuiUtility::BeginFrame();

	startTime = GetTickCount64();

	// イベント処理
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	// staticクラスのUpdate処理
	Input::Update();
	Physics2D::Update();
	InputSystem::Update();
	SceneEditor::Update();
	//GameState::Update();

	Debug::Log("%d", (int)(GetTickCount64() - startTime));
	startTime = GetTickCount64();

	// Update処理
	Scene* activeScene = SceneManager::GetActiveScene();
	if (activeScene != nullptr)
	{
		if (SceneEditor::GetIsEditMode())
		{
			activeScene->EditorUpdate();
		}
		else
		{
			activeScene->Start();
			activeScene->Update();
		}
	}

	Debug::Log("Update：%d", (int)(GetTickCount64() - startTime));
	startTime = GetTickCount64();

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

	Debug::Log("レンダリング：%d", (int)(GetTickCount64() - startTime));
	startTime = GetTickCount64();

	// ImGui描画
	// これを最後に持ってこないと、オブジェクトの下にGUIが表示されてしまう。
	ImGuiUtility::Render();

	Debug::Log("ImGui：%d", (int)(GetTickCount64() - startTime));

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
