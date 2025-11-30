#include "DemoSceneAsset.h"

#include "GameEngine.h"
#include "DemoSoundPlayer.h"
#include "DemoPhysicsManager.h"
#include "DemoMovement.h"

using namespace SceneManagement;

Scene* DemoSceneAsset::load_scene_asset_impl()
{
	Scene* scene = SceneManager::CreateScene("DemoScene");
	

	// カメラ
	GameObject* cameraText = GameObject::Create();
	cameraText->GetTransform()->SetPosition(0, -10);
	cameraText->AddComponent<TextLabel>()->SetText("Camera");
	scene->AddGameObject(cameraText);

	GameObject* camera = GameObject::Create();
	camera->tag = "MainCamera";
	camera->AddComponent<Camera>();
	camera->AddComponent<DemoMovement>();
	scene->AddGameObject(camera);


	// メッシュ
	GameObject* meshText = GameObject::Create();
	TextLabel* meshTextLabel = meshText->AddComponent<TextLabel>();
	meshTextLabel->SetText("Mesh");
	meshTextLabel->SetCanMove(true);
	meshText->GetTransform()->SetPosition(-14, 9);
	scene->AddGameObject(meshText);

	GameObject* meshObject = GameObject::Create();
	meshObject->AddComponent<SpriteRenderer>();
	meshObject->GetTransform()->SetPosition(-14, 7);
	scene->AddGameObject(meshObject);

	GameObject* meshObject2 = GameObject::Create();
	meshObject2->AddComponent<SpriteRenderer>()->SetColor(DirectX::XMFLOAT4(0, 0, 1, 1));
	meshObject2->GetTransform()->SetPosition(-14, 5);
	scene->AddGameObject(meshObject2);


	// テクスチャ
	GameObject* textureText = GameObject::Create();
	TextLabel* textureTextLabel = textureText->AddComponent<TextLabel>();
	textureTextLabel->SetText("Texture");
	textureTextLabel->SetCanMove(true);
	textureText->GetTransform()->SetPosition(-9, 9);
	scene->AddGameObject(textureText);

	Texture* texture = new Texture("./Resources/Texture/knight_m_idle_anim_f0.png");
	Texture* texture2 = new Texture("./Resources/Texture/knight_m_idle_anim_f1.png");
	Texture* texture3 = new Texture("./Resources/Texture/knight_m_idle_anim_f2.png");
	Texture* texture4 = new Texture("./Resources/Texture/knight_m_idle_anim_f3.png");

	GameObject* textureObject = GameObject::Create();
	textureObject->AddComponent<SpriteRenderer>()->SetTexture(texture);
	textureObject->GetTransform()->SetPosition(-9, 7);
	textureObject->GetTransform()->scale = Vector3(2, 2 * 28 / 16.0f, 0);
	scene->AddGameObject(textureObject);

	GameObject* textureObject2 = GameObject::Create();
	textureObject2->AddComponent<SpriteRenderer>()->SetTexture(texture2);
	textureObject2->GetTransform()->SetPosition(-9, 4);
	textureObject2->GetTransform()->scale = Vector3(2, 2 * 28 / 16.0f, 0);
	scene->AddGameObject(textureObject2);

	GameObject* textureObject3 = GameObject::Create();
	textureObject3->AddComponent<SpriteRenderer>()->SetTexture(texture3);
	textureObject3->GetTransform()->SetPosition(-9, 1);
	textureObject3->GetTransform()->scale = Vector3(2, 2 * 28 / 16.0f, 0);
	scene->AddGameObject(textureObject3);

	GameObject* textureObject4 = GameObject::Create();
	textureObject4->AddComponent<SpriteRenderer>()->SetTexture(texture4);
	textureObject4->GetTransform()->SetPosition(-9, -2);
	textureObject4->GetTransform()->scale = Vector3(2, 2 * 28 / 16.0f, 0);
	scene->AddGameObject(textureObject4);


	// アニメーション
	GameObject* animationText = GameObject::Create();
	TextLabel* animationTextLabel = animationText->AddComponent<TextLabel>();
	animationTextLabel->SetText("Animation");
	animationTextLabel->SetCanMove(true);
	animationText->GetTransform()->SetPosition(-2, 9);
	scene->AddGameObject(animationText);

	GameObject* animationObject = GameObject::Create();
	SpriteRenderer* animationSpriteRenderer = animationObject->AddComponent<SpriteRenderer>();
	animationObject->GetTransform()->SetPosition(-2, 7);
	animationObject->GetTransform()->scale = Vector3(2, 2 * 28 / 16.0f, 0);

	//Animation* animation = new Animation("Idle", animationSpriteRenderer);
	/*animation->SetAnimation(texture, 0);
	animation->SetAnimation(texture2, 0.2f);
	animation->SetAnimation(texture3, 0.4f);
	animation->SetAnimation(texture4, 0.6f);
	animation->SetAnimation(texture4, 0.8f);
	animation->SetLoopTime(true);*/

	Animator* animator = animationObject->AddComponent<Animator>();
	//animator->SetAnimation(animation);
	animator->Play("Idle");

	scene->AddGameObject(animationObject);


	// サウンド
	GameObject* soundText = GameObject::Create();
	TextLabel* soundTextLabel = soundText->AddComponent<TextLabel>();
	soundTextLabel->SetText("Sound");
	soundTextLabel->SetCanMove(true);
	soundText->GetTransform()->SetPosition(4, 9);
	scene->AddGameObject(soundText);

	AudioClip* audioClip = new AudioClip("./Resources/Sound/SE/RetroWeaponLaser03.wav");

	GameObject* soundPlayer = GameObject::Create();
	soundPlayer->GetTransform()->SetPosition(4, 7);
	AudioSource* audioSource = soundPlayer->AddComponent<AudioSource>();
	audioSource->SetAudioClip(audioClip);
	soundPlayer->AddComponent<DemoSoundPlayer>();
	scene->AddGameObject(soundPlayer);


	// 物理演算
	GameObject* physicsText = GameObject::Create();
	TextLabel* physicsTextLabel = physicsText->AddComponent<TextLabel>();
	physicsTextLabel->SetText("Physics");
	physicsTextLabel->SetCanMove(true);
	physicsText->GetTransform()->SetPosition(10, 9);
	scene->AddGameObject(physicsText);

	GameObject* demoPhysicsManager = GameObject::Create();
	demoPhysicsManager->GetTransform()->SetPosition(10, 7);
	demoPhysicsManager->AddComponent<DemoPhysicsManager>();
	scene->AddGameObject(demoPhysicsManager);


	// Image
	GameObject* imageText = GameObject::Create();
	TextLabel* imageTextLabel = imageText->AddComponent<TextLabel>();
	imageTextLabel->SetText("UI Image");
	imageTextLabel->SetCanMove(false);
	imageText->GetTransform()->SetPosition(-14, -9);
	scene->AddGameObject(imageText);

	Texture* picture = new Texture("./Resources/Texture/picture.png");
	GameObject* imageObject = GameObject::Create();
	SpriteRenderer* imageSpriteRenderer = imageObject->AddComponent<SpriteRenderer>();
	imageSpriteRenderer->SetTexture(picture);
	imageSpriteRenderer->SetCanMove(false);
	imageObject->GetTransform()->SetPosition(-14, -6);
	imageObject->GetTransform()->scale = Vector3(4, 4, 0);
	scene->AddGameObject(imageObject);


	return scene;
}