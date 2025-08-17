#include "DemoSoundPlayer.h"

#include "GameEngine.h"

using namespace SceneManagement;

void DemoSoundPlayer::Update()
{
	if (!_started)
	{
		_started = true;

		_cube->GetTransform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y);
		SceneManager::GetActiveScene()->AddGameObject(_cube);
	}

	if (_timer >= _finishTime)
	{
		_cube->GetComponent<SpriteRenderer>()->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
	}

	if (_timer >= _interval)
	{
		_timer = 0;

		_cube->GetComponent<SpriteRenderer>()->SetColor(DirectX::XMFLOAT4(1, 1, 0, 1));
		this->get_gameObject()->GetComponent<AudioSource>()->Play();
	}

	_timer += Time::GetDelataTime();
}

DemoSoundPlayer::DemoSoundPlayer()
{
	_cube->AddComponent<SpriteRenderer>();
}