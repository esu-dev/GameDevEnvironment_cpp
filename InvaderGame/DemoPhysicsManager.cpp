#include "DemoPhysicsManager.h"

#include "GameEngine.h"

void DemoPhysicsManager::Update()
{
	if (!_started)
	{
		_started = true;

		_cube->GetTransform()->SetPosition(this->get_transform()->position.Get().x, this->get_transform()->position.Get().y);
		SceneManager::GetActiveScene()->AddGameObject(_cube);

		_ground->GetTransform()->SetPosition(this->get_transform()->position.Get().x, this->get_transform()->position.Get().y - 15);
		SceneManager::GetActiveScene()->AddGameObject(_ground);
	}

	if (_timer >= _interval)
	{
		_timer = 0;

		_cube->GetComponent<Rigidbody2D>()->velocity = Vector2(0, -0.1f);
		_cube->GetTransform()->SetPosition(this->get_transform()->position.Get().x, this->get_transform()->position.Get().y);
		_cube->GetComponent<Rigidbody2D>()->SetDynamic();
	}

	_timer += EngineTime::GetDeltaTime();
}

DemoPhysicsManager::DemoPhysicsManager()
{
	_cube->AddComponent<SpriteRenderer>();
	_cube->AddComponent<BoxCollider2D>()->SetSize(Vector2(1, 1));
	_cube->AddComponent<Rigidbody2D>();

	_ground->AddComponent<SpriteRenderer>();
	_ground->AddComponent<BoxCollider2D>()->SetSize(Vector2(1, 1));
	_ground->AddComponent<Rigidbody2D>()->SetKinematic();
}