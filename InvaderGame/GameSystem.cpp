#include "GameSystem.h"

#include "DirectX.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SceneEditor.h"
#include "GameState.h"
#include "EditorCamera.h"
#include "Profiler.h"

#include "ImGuiUtility.h"

using namespace SceneManagement;

typedef void (*HELLO_WORLD)();
typedef void (*INIT_PROJ_FUNC)(Activator*);

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

	// ここでDLL読み込み
	HMODULE hModule = LoadLibrary(L"C:/Users/har14/source/repos/Dll_Test/x64/Debug/Dll_Test.dll");

	if (hModule != NULL)
	{
		 HELLO_WORLD helloWorldFunc = (HELLO_WORLD)GetProcAddress(hModule, "HelloWorld");
		 helloWorldFunc();

		 INIT_PROJ_FUNC initProjFunc = (INIT_PROJ_FUNC)GetProcAddress(hModule, "InitProj");
		 initProjFunc(Activator::Instance);
	}
}

void GameSystem::Execute()
{
	Profiler::AddFrameData(new Profiler::FrameData());
	auto* frameData = Profiler::GetLastFrameData();
	auto startTime = std::chrono::high_resolution_clock::now();

	// 背景色の設定
	const auto& bg = Camera::backgroundColor;
	float color[4] = { bg.x, bg.y, bg.z, bg.w };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	//Debug::Log("%d", (int)(GetTickCount64() - startTime));

	ImGuiUtility::BeginFrame();

	
	// イベント処理
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	// staticクラスのUpdate処理
	Input::Update();

	auto* cd_physics = new Profiler::FrameData();
	frameData->categories["Physics"] = cd_physics;
	auto st_physics = std::chrono::high_resolution_clock::now();
	Physics2D::Update();
	auto et_physics = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration_physics = et_physics - st_physics;
	cd_physics->totalTime = duration_physics.count();

	//InputSystem::Update();
	SceneEditor::Update();
	//GameState::Update();

	auto* cd_scripts = new Profiler::FrameData();
	frameData->categories["Scripts"] = cd_scripts;
	auto st_scripts = std::chrono::high_resolution_clock::now();

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
	
	auto et_scripts = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration_scripts = et_scripts - st_scripts;
	cd_scripts->totalTime = duration_scripts.count();
	

	auto st_rendering = std::chrono::high_resolution_clock::now();

	if (SceneEditor::GetIsEditMode())
	{
		Direct3D::GetInstance().StartRendering(EditorCamera::GetPosition().ToXMVECTOR(), EditorCamera::GetSize());
	}
	else
	{
		Direct3D::GetInstance().StartRendering(Camera::GetMain()->GetTransform()->position.Get().ToXMVECTOR(), Camera::GetMain()->GetSize());
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

	auto* cd_rendering = new Profiler::FrameData();
	frameData->categories["Rendering"] = cd_rendering;
	auto et_rendering = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration_rendering = et_rendering - st_rendering;
	cd_rendering->totalTime = duration_rendering.count();


	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration = endTime - startTime;
	frameData->totalTime = duration.count();

	if (!SceneEditor::GetIsEditMode())
	{
		Profiler::Render();
	}

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

void GameSystem::AddRenderingData2(int order, std::function<void()> setDataAct, std::function<void()> renderingAct)
{
	RenderingData* renderData = new RenderingData();
	renderData->order = order;
	renderData->setDataAct = setDataAct;
	renderData->function = renderingAct;

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
