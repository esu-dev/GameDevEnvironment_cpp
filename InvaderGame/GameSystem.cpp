#include "DirectX.h"

#include "GameSystem.h"

#include "SampleMonoBehaviour.h"

#include "DirectSound.h"

using namespace SceneManagement;


void GameSystem::AddDelayedExecution(std::function<void()> func)
{
	_delayedExecutionEvent.AddListener(func);
}

void GameSystem::Initialize()
{
	// static classの初期化
	Time::Initialize();
	Physics2D::Initialize();

	m_Texture.Load("./Data/Logo.png");

	D3D.ChangeMode_2D();

	Scene* currentScene = SceneManager::CreateScene("test");
	SceneManager::CreateScene("test2");
	SceneManager::SetActiveScene(currentScene);

	GameObject* title = new GameObject();
	title->GetTransform()->SetPosition(0, 4);
	title->AddComponent<TextLabel>()->SetText("title");
	title->GetComponent<TextLabel>()->SetFontSize(32);
	currentScene->AddGameObject(title);

	//_gameObjectVector.push_back(new GameObject());
	//_gameObjectVector[1]->AddComponent<TextLabel>()->SetText("I won!");
	//_gameObjectVector[1]->GetTransform()->SetPosition(0, 200);
	//_gameObjectVector[1]->GetComponent<TextLabel>()->SetFontSize(32);

	///*_gameObjectVector.push_back(new GameObject());
	//_gameObjectVector[2]->GetTransform()->position = Vector3(100, 0, 0);
	//_gameObjectVector[2]->GetTransform()->scale = { 2, 1, 1 };
	//_gameObjectVector[2]->AddComponent<SpriteRenderer>()->SetTexture(&m_Texture);*/


	Texture* mainShipTexture = new Texture();
	mainShipTexture->Load("./Resources/MainShip/FullHealth.png");
	Texture* mainShipTexture_SlightDamage = new Texture("./Resources/MainShip/SlightDamage.png");

	AudioClip* mainShipAudioClip = new AudioClip("./Resources/Sound/SE/RetroWeaponLaser03.wav");

	_testObject = new GameObject();
	_testObject->AddComponent<SampleMonoBehaviour>();
	_testObject->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_testObject->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	_testObject->AddComponent<Rigidbody2D>()->SetUseGravity(false);
	_testObject->AddComponent<AudioSource>()->SetAudioClip(mainShipAudioClip);
	_testObject->GetTransform()->position = Vector3(-2, 0, 0);
	_testObject->GetTransform()->scale = { 2, 2, 0.0f };

	Animation* mainShipAnimation = new Animation("SlightDamage", _testObject->GetComponent<SpriteRenderer>());
	mainShipAnimation->SetAnimation(mainShipTexture, 0);
	mainShipAnimation->SetAnimation(mainShipTexture_SlightDamage, 1);
	_testObject->AddComponent<Animator>()->SetAnimation(mainShipAnimation);

	currentScene->AddGameObject(_testObject);


	GameObject* wall = new GameObject();
	wall->AddComponent<SpriteRenderer>();
	wall->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	Rigidbody2D* rb_ship2 = wall->AddComponent<Rigidbody2D>();
	//rb_ship2->SetUseGravity(false);
	rb_ship2->SetFreeze();
	wall->GetTransform()->position = Vector3(-2, -4, 0);
	wall->GetTransform()->scale = { 2, 2, 0 }; // コンストラクタの呼び出し

	currentScene->AddGameObject(wall);

	GameObject* rect = new GameObject();
	rect->AddComponent<SpriteRenderer>();
	rect->GetTransform()->position = Vector3(0, 0, 0);
	rect->GetTransform()->scale = { 2, 2, 0.0f };

	currentScene->AddGameObject(rect);

	AudioClip* audioClip = new AudioClip("./Resources/Sound/BGM/SpaceshipShooter.wav");
	BGM = new GameObject();
	AudioSource* audioSource = BGM->AddComponent<AudioSource>();
	audioSource->SetAudioClip(audioClip);
	audioSource->Play();

	Debug::Log(L"Play!");

	GameObject* title2 = new GameObject();
	title2->GetTransform()->SetPosition(0, 4);
	title2->AddComponent<TextLabel>()->SetText("Game Scene");
	title2->GetComponent<TextLabel>()->SetFontSize(32);
	SceneManager::GetSceneAt(1)->AddGameObject(title2);

	// Start処理
	SceneManager::LoadScene("test");
}

void GameSystem::Execute()
{
	_delayedExecutionEvent.Invoke();

	// イベント処理
	OnUpdateListener.Invoke();

	Physics2D::Update();

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
	else if (Input::GetKeyDown('Q'))
	{
		BGM->GetComponent<AudioSource>()->Stop();
	}
	else
	{
		_testObject->GetComponent<Rigidbody2D>()->SetVelocity(Vector2(0, 0));
	}

    D3D.m_swapChain->Present(1, 0);
}
