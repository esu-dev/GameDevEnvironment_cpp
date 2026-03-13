#include "MeshRenderer.h"

#include "Record.h"
#include "GameSystem.h"
#include "Transform.h"
#include "Vector3.h"

MeshRenderer::MeshRenderer()
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

void MeshRenderer::Start()
{
	
}

void MeshRenderer::Update()
{
	GameSystem::GetInstance().AddRenderingFunc([&]() -> void { Render(); });
}

void MeshRenderer::EditorUpdate()
{
	GameSystem::GetInstance().AddRenderingFunc([&]() -> void { Render(); });
}

void MeshRenderer::Render()
{
	// メッシュデータを設定
	Direct3D::GetInstance().SetMeshData(_mesh->GetVertices(), _mesh->GetIndices());

	// 座標データを設定
	Transform* t = this->GetTransform();
	Direct3D::GetInstance().SetObjectData(
		{ t->position.Get().x, t->position.Get().y, t->position.Get().z },
		{ t->scale.x, t->scale.y, t->scale.z },
		t->rotation,
		{ 1, 1, 1, 1 }
	);

	// 描画する
	Direct3D::GetInstance().Draw3D();
}