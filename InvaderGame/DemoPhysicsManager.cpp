#include "DemoPhysicsManager.h"

#include "GameEngine.h"

void DemoPhysicsManager::Update()
{
	if (!_started)
	{
		_started = true;

		_cube->GetTransform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y);
		SceneManager::GetActiveScene()->AddGameObject(_cube);

		_ground->GetTransform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y - 15);
		SceneManager::GetActiveScene()->AddGameObject(_ground);
	}

	if (_timer >= _interval)
	{
		_timer = 0;

		_cube->GetTransform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y);
	}

	_timer += Time::GetDelataTime();
}

DemoPhysicsManager::DemoPhysicsManager()
{
	_cube->AddComponent<SpriteRenderer>();
	_cube->AddComponent<BoxCollider2D>()->SetSize(Vector2(1.275, 1.275));
	_cube->AddComponent<Rigidbody2D>();

	_ground->AddComponent<SpriteRenderer>();
	_ground->AddComponent<BoxCollider2D>()->SetSize(Vector2(1.275, 1.275));
	_ground->AddComponent<Rigidbody2D>()->SetKinematic();
}