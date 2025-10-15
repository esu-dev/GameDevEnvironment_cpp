# include "SampleSceneAssests.h"

#include "GameEngine.h"
#include "SampleMonoBehaviour.h"
#include "SamplePrefabAsset.h"

using namespace SceneManagement;

Scene* SampleSceneAssets::load_scene_asset_impl()
{
	Scene* sampleScene = SceneManager::CreateScene("SampleScene");

	GameObject* camera = GameObject::Create();
	camera->AddComponent<Camera>();
	camera->tag = "MainCamera";
	sampleScene->AddGameObject(camera);


	GameObject* canvasObject = GameObject::Create();
	canvasObject->AddComponent<Canvas>();
	sampleScene->AddGameObject(canvasObject);

	GameObject* title = GameObject::Create();
	title->GetTransform()->SetPosition(0, 4);
	title->AddComponent<TextLabel>()->SetText("title");
	title->GetComponent<TextLabel>()->SetFontSize(32);
	sampleScene->AddGameObject(title);


	GameObject* title2 = GameObject::Create();
	title2->GetTransform()->SetPosition(0, 5);
	title2->AddComponent<TextLabel>()->SetText("title");
	title2->GetComponent<TextLabel>()->SetFontSize(32);
	title2->GetComponent<TextLabel>()->SetTextAlign(TextLabel::TextAlign::Left);
	sampleScene->AddGameObject(title2);


	GameObject* title3 = GameObject::Create();
	title3->GetTransform()->SetPosition(0, 6);
	title3->AddComponent<TextLabel>()->SetText("title");
	title3->GetComponent<TextLabel>()->SetFontSize(32);
	title3->GetComponent<TextLabel>()->SetTextAlign(TextLabel::TextAlign::Right);
	sampleScene->AddGameObject(title3);


	Texture* mainShipTexture = new Texture();
	mainShipTexture->Load("./Resources/MainShip/FullHealth.png");

	AudioClip* mainShipAudioClip = new AudioClip("./Resources/Sound/SE/RetroWeaponLaser03.wav");

	GameObject* _testObject = GameObject::Create();
	_testObject->AddComponent<SampleMonoBehaviour>();
	_testObject->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_testObject->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	_testObject->AddComponent<Rigidbody2D>();
	Rigidbody2D* r = _testObject->GetComponent<SpriteRenderer>()->GetComponent<Rigidbody2D>();
	r->SetUseGravity(false);
	//r->SetVelocity(Vector2(0, -0.1));
	_testObject->GetTransform()->position = Vector3(8, 0, 0);
	_testObject->GetTransform()->scale = { 2, 2, 0.0f };

	sampleScene->AddGameObject(_testObject);


	GameObject* wall = GameObject::Create();
	wall->AddComponent<SpriteRenderer>();
	//wall->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	//Rigidbody2D* rb_ship2 = wall->AddComponent<Rigidbody2D>();
	//rb_ship2->SetUseGravity(false);
	//rb_ship2->SetFreeze();
	wall->GetTransform()->position = Vector3(-10, 10, 0);
	wall->GetTransform()->scale = { 2, 2, 0 }; // コンストラクタの呼び出し

	sampleScene->AddGameObject(wall);

	GameObject* samplePrefabGameObject = SamplePrefabAsset::load();
	sampleScene->AddGameObject(samplePrefabGameObject);
	samplePrefabGameObject->GetTransform()->position = samplePrefabGameObject->GetTransform()->position - Vector3(3, 3, 0).GetNormalized();
	//samplePrefabGameObject->GetTransform()->position = Vector3::zero;

	Debug::Log("あいう%s", "aiueoかき");
	//Debug::Log(L"aiueoあいうえお");


	/*
	Texture* mainShipTexture = new Texture();
	mainShipTexture->Load("./Resources/MainShip/FullHealth.png");
	Texture* mainShipTexture_SlightDamage = new Texture("./Resources/MainShip/SlightDamage.png");

	AudioClip* mainShipAudioClip = new AudioClip("./Resources/Sound/SE/RetroWeaponLaser03.wav");

	GameObject* _testObject = GameObject::Create();
	_testObject->AddComponent<SampleMonoBehaviour>();
	_testObject->AddComponent<SpriteRenderer>()->SetTexture(mainShipTexture);
	_testObject->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	_testObject->AddComponent<Rigidbody2D>()->SetUseGravity(false);
	_testObject->AddComponent<AudioSource>()->SetAudioClip(mainShipAudioClip);
	_testObject->GetTransform()->position = Vector3(8, 0, 0);
	_testObject->GetTransform()->scale = { 2, 2, 0.0f };

	Animation* mainShipAnimation = new Animation("SlightDamage", _testObject->GetComponent<SpriteRenderer>());
	mainShipAnimation->SetAnimation(mainShipTexture, 0);
	mainShipAnimation->SetAnimation(mainShipTexture_SlightDamage, 1);
	_testObject->AddComponent<Animator>()->SetAnimation(mainShipAnimation);

	sampleScene->AddGameObject(_testObject);


	GameObject* wall = GameObject::Create();
	wall->AddComponent<SpriteRenderer>();
	wall->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
	Rigidbody2D* rb_ship2 = wall->AddComponent<Rigidbody2D>();
	//rb_ship2->SetUseGravity(false);
	rb_ship2->SetFreeze();
	wall->GetTransform()->position = Vector3(-2, -4, 0);
	wall->GetTransform()->scale = { 2, 2, 0 }; // コンストラクタの呼び出し

	sampleScene->AddGameObject(wall);

	GameObject* rect = GameObject::Create();
	rect->AddComponent<SpriteRenderer>();
	rect->GetTransform()->position = Vector3(0, 0, 0);
	rect->GetTransform()->scale = { 2, 2, 0.0f };

	sampleScene->AddGameObject(rect);


	AudioClip* audioClip = new AudioClip("./Resources/Sound/BGM/SpaceshipShooter.wav");
	GameObject* BGM = GameObject::Create();
	AudioSource* audioSource = BGM->AddComponent<AudioSource>();
	audioSource->SetAudioClip(audioClip);
	audioSource->SetVolume(0.2f);
	audioSource->Play();
	*/


	return sampleScene;
}