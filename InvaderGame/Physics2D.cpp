#include "Physics2D.h"

#include "GameEngine.h"

using namespace SceneManagement;

Physics2D::LibraryType Physics2D::_libraryType = Physics2D::LibraryType::Original;
b2Vec2 Physics2D::_gravity = { 0.0f, -9.81f };
b2World Physics2D::_world = { _gravity };


Physics2D::LibraryType Physics2D::GetLibraryType()
{
	return _libraryType;
}

void Physics2D::Initialize()
{
	MyContactListener* contactListener = new MyContactListener();
	_world.SetContactListener(contactListener);
}

void Physics2D::Update()
{
	if (_libraryType == LibraryType::Original)
	{
		std::vector<std::pair<Collider2D*, Collider2D*>> collisionPairVector;

		auto gameObjectVector = SceneManager::GetActiveScene()->GetGameObjectVector();
		
		// 衝突検出（ブロードフェーズ）
		for (int i = 0; i < gameObjectVector.size(); i++)
		{
			Collider2D* colliderA = gameObjectVector[i]->GetComponent<Collider2D>();
			if (colliderA == nullptr) continue;

			for (int j = i + 1; j < gameObjectVector.size(); j++)
			{
				Collider2D* colliderB = gameObjectVector[j]->GetComponent<Collider2D>();
				if (colliderB == nullptr) continue;

				if (colliderA->IsAABB_Collided(colliderB))
				{
					collisionPairVector.push_back({ colliderA, colliderB });
				}
			}
		}

		// 衝突検出（ナローフェーズ）


		return;
	}

	float timeStep = Time::FixedDeltaTime;
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	_world.Step(timeStep, velocityIterations, positionIterations);
}

void MyContactListener::BeginContact(b2Contact* contact)
{
	Debug::Log(L"begin contact.");

	// 相手のb2bodyを取得
	b2Body* body_A = contact->GetFixtureA()->GetBody();
	b2Body* body_B = contact->GetFixtureB()->GetBody();

	Scene* scene = SceneManager::GetActiveScene();

	// 関数内関数
	// b2bodyを持つGameObjectを取得する
	std::function<GameObject* (b2Body*)> FindGameObjectWith = [&](b2Body* body)
	{
		GameObject* gameObject = std_extension::Find<GameObject*>(scene->GetGameObjectVector(), [&](GameObject* x)
			{
				Collider2D* collider = x->GetComponent<Collider2D>();
				if (collider == nullptr)
				{
					return false;
				}
				return collider->Getb2Body() == body;
			});
		
		return gameObject;
	};
	
	GameObject* gameObject = FindGameObjectWith(body_A);
	MonoBehaviour* monoBehaviour = gameObject->GetComponent<MonoBehaviour>();

	if (monoBehaviour == nullptr)
	{
		return;
	}

	// Collision2Dの作成
	monoBehaviour->OnCollisionEnter2D(new Collision2D(FindGameObjectWith(body_B)->GetComponent<Collider2D>()));
}

void MyContactListener::EndContact(b2Contact* contact)
{
	Debug::Log(L"end contact.");
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	//Debug::Log(L"pre solve.");
}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	//Debug::Log(L"post solve.");
}