#include "DemoPhysicsSceneAsset.h"

#include "GameEngine.h"

using namespace SceneManagement;

Scene* DemoPhysicsSceneAsset::load_scene_asset_impl()
{
	Scene* scene = SceneManager::CreateScene("DemoPhysicsScene");

	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->tag = "MainCamera";
	scene->AddGameObject(camera);

	GameObject* cube = new GameObject();
	cube->GetTransform()->SetPosition(3, 0);
	cube->AddComponent<SpriteRenderer>();
	cube->AddComponent<BoxCollider2D>();
	cube->AddComponent<Rigidbody2D>();
	scene->AddGameObject(cube);

	Texture* circleTex = new Texture("Resources/Texture/Circle.png");

	GameObject* circle = new GameObject();
	circle->AddComponent<SpriteRenderer>()->SetTexture(circleTex);
	circle->AddComponent<CircleCollider2D>();
	circle->AddComponent<Rigidbody2D>();
	scene->AddGameObject(circle);
	
	return scene;
}