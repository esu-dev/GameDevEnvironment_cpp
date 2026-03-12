#include "MeshRenderer.h"

#include "GameSystem.h"

void MeshRenderer::Start()
{
	_mesh = new Mesh();
	if (_mesh->Load("./Resources/Fbx/cube.fbx"))
	{
		Debug::Log(L"メッシュのロードが完了しました．");
	}
	else
	{
		Debug::Log(L"メッシュのロードに失敗しました．");
	}
}

void MeshRenderer::Update()
{
	auto render = []() -> void {
		// メッシュデータをGPUに送る

		// 描画する
	};

	GameSystem::GetInstance().AddRenderingFunc(render);
}