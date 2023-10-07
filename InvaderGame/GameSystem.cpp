#include "framework.h"

#include "DirectX.h"

#include "GameSystem.h"

void GameSystem::Initialize()
{
	m_Texture.Load("./Data/Logo.png");

	D3D.ChangeMode_2D();

	_gameObjectVector.push_back(GameObject());
	_gameObjectVector[0].AddComponent<TextLabel>().SetText("clear!");

	_gameObjectVector.push_back(GameObject());
	_gameObjectVector[1].AddComponent<SpriteRenderer>().SetTexture(&m_Texture);
}

void GameSystem::Execute()
{
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	for (GameObject gameObject : _gameObjectVector)
	{
		gameObject.Update();
	}

    D3D.m_swapChain->Present(1, 0);
}

void GameSystem::AddSquare(float x, float y, float w, float h)
{
	//_squareVector.push_back(Square(m_Texture, x, y, w, h));
}