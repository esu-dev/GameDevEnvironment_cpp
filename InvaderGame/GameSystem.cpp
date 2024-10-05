#include "DirectX.h"

#include "GameSystem.h"

#include "SceneManager.h"

using namespace SceneManagement;


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

	Scene* currentScene = SceneManager::CreateScene("test");
	SceneManager::CreateScene("test2");
	SceneManager::SetActiveScene(currentScene);

	GameObject* title = new GameObject();
	title->transform->SetPosition(0, 4);
	title->AddComponent<TextLabel>()->SetText("title");
	title->GetComponent<TextLabel>()->SetFontSize(32);
	currentScene->AddGameObject(title);

	//_gameObjectVector.push_back(new GameObject());
	//_gameObjectVector[1]->AddComponent<TextLabel>()->SetText("I won!");
	//_gameObjectVector[1]->transform->SetPosition(0, 200);
	//_gameObjectVector[1]->GetComponent<TextLabel>()->SetFontSize(32);

	///*_gameObjectVector.push_back(new GameObject());
	//_gameObjectVector[2]->transform->position = Vector3(100, 0, 0);
	//_gameObjectVector[2]->transform->scale = { 2, 1, 1 };
	//_gameObjectVector[2]->AddComponent<SpriteRenderer>()->SetTexture(&m_Texture);*/

	Texture* mainShipTexture = new Texture();
	mainShipTexture->Load("./Resources/MainShip/FullHealth.png");
	Texture* mainShipTexture_SlightDamage = new Texture("./Resources/MainShip/SlightDamage.png");

	_testObject = new GameObject();
	_testObject->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_testObject->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	_testObject->AddComponent<Rigidbody2D>()->SetUseGravity(false);
	_testObject->transform->position = Vector3(-2, 0, 0);
	_testObject->transform->scale = { 2, 2, 0.0f };

	Animation* mainShipAnimation = new Animation("SlightDamage", _testObject->GetComponent<SpriteRenderer>());
	mainShipAnimation->SetAnimation(mainShipTexture, 0);
	mainShipAnimation->SetAnimation(mainShipTexture_SlightDamage, 1);
	_testObject->AddComponent<Animator>()->SetAnimation(mainShipAnimation);

	currentScene->AddGameObject(_testObject);

	GameObject* wall = new GameObject();
	wall->AddComponent<SpriteRenderer>();
	wall->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	Rigidbody2D* rb_ship2 = wall->AddComponent<Rigidbody2D>();
	rb_ship2->SetUseGravity(false);
	//rb_ship2->SetFreeze();
	wall->transform->position = Vector3(-2, -4, 0);
	wall->transform->scale = { 2, 2, 0 }; // コンストラクタの呼び出し

	currentScene->AddGameObject(wall);

	GameObject* rect = new GameObject();
	rect->AddComponent<SpriteRenderer>();
	rect->transform->position = Vector3(0, 0, 0);
	rect->transform->scale = { 2, 2, 0.0f };

	currentScene->AddGameObject(rect);


	GameObject* title2 = new GameObject();
	title2->transform->SetPosition(0, 4);
	title2->AddComponent<TextLabel>()->SetText("Game Scene");
	title2->GetComponent<TextLabel>()->SetFontSize(32);
	SceneManager::GetSceneAt(1)->AddGameObject(title2);

	// Start処理
	SceneManager::LoadScene("test");
}

void GameSystem::Execute()
{
	// イベント処理
	OnUpdateListener.Invoke();

    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.m_deviceContext->ClearRenderTargetView(D3D.m_backBufferView.Get(), color);
	
	// Update処理
	SceneManager::GetActiveScene()->Update();

	if (Input::GetKeyDown('L'))
	{
		Debug::Log(L"keydown");
		//_gameObjectVector.back()->GetComponent<Animator>()->Play("SlightDamage");
		SceneManager::LoadScene("test2");
	}

	if (Input::GetKey('W'))
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(0, 4.0f));
	}
	else if (Input::GetKey('A'))
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(-4.0f, 0));
	}
	else if (Input::GetKey('S'))
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(0, -4.0f));
	}
	else if (Input::GetKey('D'))
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(4.0f, 0));
	}
	else
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(0, 0));
	}

	float timeStep = Time::FixedDeltaTime;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	_world.Step(timeStep, velocityIterations, positionIterations);

    D3D.m_swapChain->Present(1, 0);
}

void GameSystem::AddSquare(float x, float y, float w, float h)
{
	//_squareVector.push_back(Square(m_Texture, x, y, w, h));
}
