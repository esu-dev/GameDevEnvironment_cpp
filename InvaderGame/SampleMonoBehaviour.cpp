#include "SampleMonoBehaviour.h"

#include "GameEngine.h"

SampleMonoBehaviour::SampleMonoBehaviour()
{
	_test->AddComponent<SpriteRenderer>();
	_test->GetTransform()->position = Vector3(-5, 0, 0);

	fpsText->AddComponent<TextLabel>()->SetText("fps: "s + std::to_string(60));
	fpsText->GetTransform()->SetPosition(-5, 4);
}

void SampleMonoBehaviour::OnCollisionEnter2D(Collision2D* collision)
{
	/*collision->GetCollider()->gameObject->SetActive(false);

	this->gameObject->GetComponent<AudioSource>()->Play();*/
}

void SampleMonoBehaviour::Start()
{
	Debug::Log(L"SampleMono Startd");

	SceneManagement::SceneManager::GetActiveScene()->AddGameObject(_test);
	SceneManagement::SceneManager::GetActiveScene()->AddGameObject(fpsText);
}

void SampleMonoBehaviour::Update()
{
	if (Input::GetKeyDown(VK_SPACE))
	{
		GameObject* _testObject = new GameObject();
		_testObject->AddComponent<SpriteRenderer>();
		_testObject->AddComponent<BoxCollider2D>()->SetSize(Vector2(2.0f, 2.0f));
		Rigidbody2D* r = _testObject->AddComponent<Rigidbody2D>();
		_testObject->GetTransform()->position = Vector3(8, 5, 0);
		_testObject->GetTransform()->scale = { 2, 2, 0.0f };

		SceneManagement::SceneManager::GetActiveScene()->AddGameObject(_testObject);

		Destroy(_test);
		Destroy(_testObject);
	}

	Camera::get_main()->get_transform()->position.Get().x += EngineTime::GetDelataTime();

	fpsText->GetComponent<TextLabel>()->SetText("fps: "s + std::to_string(1 / EngineTime::GetDelataTime()));
}