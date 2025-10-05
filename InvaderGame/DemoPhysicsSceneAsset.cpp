#include "DemoPhysicsSceneAsset.h"

#include "Record.h"
#include "GameEngine.h"

using namespace SceneManagement;

Scene* DemoPhysicsSceneAsset::load_scene_asset_impl()
{
	Scene* scene = SceneManager::CreateScene("DemoPhysicsScene");


	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->tag = "MainCamera";
	scene->AddGameObject(camera);


	Texture* circleTex = new Texture("Resources/Texture/Circle.png");

	/*
	{
		GameObject* circle = new GameObject();
		circle->AddComponent<SpriteRenderer>()->SetTexture(circleTex);
		circle->AddComponent<CircleCollider2D>();
		circle->AddComponent<Rigidbody2D>();
		circle->GetTransform()->SetPosition(0, 0);
		scene->AddGameObject(circle);
	}//*/

	{
		GameObject* circle = new GameObject();
		circle->AddComponent<SpriteRenderer>()->SetTexture(circleTex);
		circle->AddComponent<CircleCollider2D>();
		circle->AddComponent<Rigidbody2D>();
		circle->GetTransform()->SetPosition(1.5f, 0);
		scene->AddGameObject(circle);
	}

	//*
	{
		GameObject* cube = new GameObject();
		cube->GetTransform()->SetPosition(-4.0f, 0);
		cube->AddComponent<SpriteRenderer>();
		cube->AddComponent<BoxCollider2D>();
		cube->AddComponent<Rigidbody2D>()->velocity = Vector2(2, 0);
		scene->AddGameObject(cube);
	}//*/

	{
		GameObject* cube = new GameObject();
		cube->GetTransform()->SetPosition(0, -5);
		cube->GetTransform()->rotation = Quaternion::AngleAxis(0, Vector3(0, 0, 1));
		cube->GetTransform()->scale = Vector3(10, 1, 0);
		cube->AddComponent<SpriteRenderer>();
		cube->AddComponent<BoxCollider2D>();
		cube->AddComponent<Rigidbody2D>()->SetKinematic();
		scene->AddGameObject(cube);
	}

	/*/
	{
		GameObject* cube = new GameObject();
		cube->GetTransform()->SetPosition(-5.0f, 0);
		cube->AddComponent<SpriteRenderer>();
		cube->AddComponent<BoxCollider2D>();
		cube->AddComponent<Rigidbody2D>();
		scene->AddGameObject(cube);
	}//*/

	{
		GameObject* cube = new GameObject();
		cube->GetTransform()->SetPosition(-5.75f, -5);
		cube->AddComponent<SpriteRenderer>();
		cube->AddComponent<BoxCollider2D>();
		cube->AddComponent<Rigidbody2D>()->SetKinematic();
		scene->AddGameObject(cube);
	}

	return scene;
}