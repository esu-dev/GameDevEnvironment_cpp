#include "framework.h"

#include "DirectX.h"

#include "GameSystem.h"
#include "Transform.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"

void GameSystem::Initialize()
{
	m_Texture.Load("./Data/Logo.png");

	D3D.ChangeMode_2D();

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[0]->transform->SetPosition(-100, 0);
	_gameObjectVector[0]->AddComponent<TextLabel>()->SetText("clear");

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[1]->AddComponent<TextLabel>()->SetText("I won!");
	_gameObjectVector[1]->transform->SetPosition(0, 200);
	_gameObjectVector[1]->GetComponent<TextLabel>()->SetFontSize(32);

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[2]->transform->position = Vector3(100, 0, 0);
	_gameObjectVector[2]->transform->scale = { 2, 1, 1 };
	_gameObjectVector[2]->AddComponent<SpriteRenderer>()->SetTexture(&m_Texture);
}

void GameSystem::Execute()
{
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	for (GameObject* gameObject : _gameObjectVector)
	{
		gameObject->Update();
	}

    D3D.m_swapChain->Present(1, 0);
}

void GameSystem::AddSquare(float x, float y, float w, float h)
{
	//_squareVector.push_back(Square(m_Texture, x, y, w, h));
}