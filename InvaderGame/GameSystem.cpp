#include "GameSystem.h"

#include "DirectX.h"
#include "TimeMeasurer.h"
#include "GameEngine.h"

#include "AssetManager.h"
#include "SceneEditor.h"
#include "GameState.h"
#include "EditorCamera.h"
#include "Profiler.h"

#include "ImGuiUtility.h"

using namespace SceneManagement;

typedef void (*HELLO_WORLD)();
typedef std::string(*GET_STRING_FUNC)();
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

	// 描画機能の初期化
	Direct3D::GetInstance().InitMode2D();
	Direct3D::GetInstance().InitMode3D();


	// ここでDLL読み込み
	HMODULE hModule = LoadLibrary(L"C:/Users/har14/source/repos/Dll_Test/x64/Debug/Dll_Test.dll");

	if (hModule != NULL)
	{
		 HELLO_WORLD helloWorldFunc = (HELLO_WORLD)GetProcAddress(hModule, "HelloWorld");
		 helloWorldFunc();

		 GET_STRING_FUNC getStringFunc = (GET_STRING_FUNC)GetProcAddress(hModule, "GetString");
		 Debug::Log("GetStringFunc: %s", getStringFunc().c_str());

		 INIT_PROJ_FUNC initProjFunc = (INIT_PROJ_FUNC)GetProcAddress(hModule, "InitProj");
		 initProjFunc(Activator::Instance);
	}
}

void GameSystem::Execute()
{
	// プロファイラの初期化
	Profiler::AddFrameData(new Profiler::FrameData());
	auto* frameData = Profiler::GetLastFrameData();
	auto startTime = std::chrono::high_resolution_clock::now();

	// 背景色の設定
	const auto& bg = Camera::backgroundColor;
	float color[4] = { bg.x, bg.y, bg.z, bg.w };
	D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);

	// ImGuiの初期化
	ImGuiUtility::BeginFrame();

	
	// イベント処理
	_delayedExecutionEvent.Invoke();
	_delayedExecutionEvent.RemoveAllListener();
	OnUpdateListener.Invoke();

	// staticクラスのUpdate処理
	Input::Update();

	// 物理演算
	auto* cd_physics = new Profiler::FrameData();
	frameData->categories["Physics"] = cd_physics;
	auto st_physics = std::chrono::high_resolution_clock::now();
	Physics2D::Update();
	auto et_physics = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration_physics = et_physics - st_physics;
	cd_physics->totalTime = duration_physics.count();

	// staticクラスのUpdate処理2
	// 分離させている意味は不明
	//InputSystem::Update();
	SceneEditor::Update();
	//GameState::Update();

	
	// ScriptsのUpdate処理
	auto* cd_scripts = new Profiler::FrameData();
	frameData->categories["Scripts"] = cd_scripts;
	auto st_scripts = std::chrono::high_resolution_clock::now();

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
	

	// レンダリング -------------------------------------------------------
	auto* cd_rendering = new Profiler::FrameData();
	frameData->categories["Rendering"] = cd_rendering;
	auto st_rendering = std::chrono::high_resolution_clock::now();

	// カメラ位置の転送
	auto* subCD_camSetting = new Profiler::FrameData();
	cd_rendering->categories["CamSetting"] = subCD_camSetting;
	TimeMeasurer::Start();
	DirectX::XMVECTOR camPos;
	int camSize = 1;
	if (SceneEditor::GetIsEditMode())
	{
		camPos = EditorCamera::GetPosition().ToXMVECTOR();
		camSize = EditorCamera::GetSize();
	}
	else
	{
		camPos = Camera::GetMain()->GetTransform()->position.Get().ToXMVECTOR();
		camSize = Camera::GetMain()->GetSize();
	}
	Direct3D::GetInstance().SetCamMat2D(camPos, camSize);
	Direct3D::GetInstance().SetCamMat3D(camPos);
	subCD_camSetting->totalTime = TimeMeasurer::end();


	// 登録されたレンダリング関数を order 順に実行する（２D）
	auto* subCD_callFunc = new Profiler::FrameData();
	cd_rendering->categories["CallFunc"] = subCD_callFunc;
	TimeMeasurer::Start();

	for (RenderingData* rd : _renderingDataVector)
	{
		if (rd && rd->function)
		{
			rd->function();
		}
		delete rd;
	}
	_renderingDataVector.clear();

	// 登録されたレンダリング関数を実行（3D）
	for (auto& func : _renderingFuncVec)
	{
		func();
	}
	_renderingFuncVec.clear();

	subCD_callFunc->totalTime = TimeMeasurer::end();


	// プロファイラへデータ転送
	auto et_rendering = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration_rendering = et_rendering - st_rendering;
	cd_rendering->totalTime = duration_rendering.count();
	// -----------------------------------------------------------------------------------------------


	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> duration = endTime - startTime;
	frameData->totalTime = duration.count();


	// プロファイラの描画
	if (!SceneEditor::GetIsEditMode())
	{
		Profiler::Render();
	}

	// ImGui描画
	// これを最後に持ってこないと、オブジェクトの下にGUIが表示されてしまう。
	ImGuiUtility::Render();

	// 画面更新
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

void GameSystem::AddRenderingFunc(std::function<void()> func)
{

}