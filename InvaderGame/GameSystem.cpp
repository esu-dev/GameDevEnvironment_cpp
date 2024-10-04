#include "DirectX.h"

#include "GameSystem.h"

b2World* GameSystem::GetBox2DWorld()
{
	return &_world;
}

void GameSystem::Initialize()
{
	// static classの初期化
	Time::Initialize();

	m_Texture.Load("./Data/Logo.png");

	D3D.ChangeMode_2D();

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[0]->transform->SetPosition(-100, 0);
	_gameObjectVector[0]->AddComponent<TextLabel>()->SetText("clear");

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[1]->AddComponent<TextLabel>()->SetText("I won!");
	_gameObjectVector[1]->transform->SetPosition(0, 200);
	_gameObjectVector[1]->GetComponent<TextLabel>()->SetFontSize(32);

	/*_gameObjectVector.push_back(new GameObject());
	_gameObjectVector[2]->transform->position = Vector3(100, 0, 0);
	_gameObjectVector[2]->transform->scale = { 2, 1, 1 };
	_gameObjectVector[2]->AddComponent<SpriteRenderer>()->SetTexture(&m_Texture);*/

	Texture* mainShipTexture = new Texture();
	mainShipTexture->Load("./Resources/MainShip/FullHealth.png");
	Texture* mainShipTexture_SlightDamage = new Texture("./Resources/MainShip/SlightDamage.png");

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector.back()->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_gameObjectVector.back()->AddComponent<BoxCollider2D>();
	_gameObjectVector.back()->AddComponent<Rigidbody2D>();
	_gameObjectVector.back()->transform->scale = { 0.96, 0.96, 0 }; // コンストラクタの呼び出し

	Animation* mainShipAnimation = new Animation("SlightDamage", _gameObjectVector.back()->GetComponent<SpriteRenderer>());
	mainShipAnimation->SetAnimation(mainShipTexture, 0);
	mainShipAnimation->SetAnimation(mainShipTexture_SlightDamage, 1);
	_gameObjectVector.back()->AddComponent<Animator>()->SetAnimation(mainShipAnimation);


	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector.back()->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_gameObjectVector.back()->AddComponent<BoxCollider2D>();
	_gameObjectVector.back()->AddComponent<Rigidbody2D>()->SetUseGravity(false);
	_gameObjectVector.back()->transform->position = Vector3(0, -100, 0);
	_gameObjectVector.back()->transform->scale = { 0.96, 0.96, 0 }; // コンストラクタの呼び出し

	_gameObjectVector.push_back(new GameObject());
	_gameObjectVector.back()->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_gameObjectVector.back()->transform->position = Vector3(100, -100, 0);
	_gameObjectVector.back()->transform->scale = { 0.96, 0.96, 0 }; // コンストラクタの呼び出し

	// Start処理
	for (GameObject* gameObject : _gameObjectVector)
	{
		gameObject->Start();
	}
}

void GameSystem::Execute()
{
	// イベント処理
	OnUpdateListener.Invoke();

    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	// Update処理
	for (GameObject* gameObject : _gameObjectVector)
	{
		gameObject->Update();
	}

	if (Input::GetKeyDown('A'))
	{
		Debug::Log(L"keydown");
		_gameObjectVector.back()->GetComponent<Animator>()->Play("SlightDamage");
	}

	//Debug::Log(L"%u", GetTickCount64());

	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	_world.Step(timeStep, velocityIterations, positionIterations);

	// 処理されているなら、その処理結果だけ用いればよい。

    D3D.m_swapChain->Present(1, 0);
}

void GameSystem::AddSquare(float x, float y, float w, float h)
{
	//_squareVector.push_back(Square(m_Texture, x, y, w, h));
}
