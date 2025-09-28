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
		static float g = 9.81f;
		static float e = 0.7f;

		auto gameObjectVector = SceneManager::GetActiveScene()->GetGameObjectVector();

		// 重力の適用
		for (GameObject* go : gameObjectVector)
		{
			if (Rigidbody2D* rigidbody = go->GetComponent<Rigidbody2D>())
			{
				if (rigidbody->isKinematic) continue;

				rigidbody->velocity += Vector2(0, -g) * Time::FixedDeltaTime;
			}
		}


		std::vector<std::pair<Collider2D*, Collider2D*>> collisionPairVector;
		
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


		std::vector<Collision2D*> collisionVector;

		// 衝突検出（ナローフェーズ）
		for (auto collisionPair : collisionPairVector)
		{
			if (BoxCollider2D* boxCollider = dynamic_cast<BoxCollider2D*>(collisionPair.second))
			{
				Collision2D* collision = new Collision2D(collisionPair.first, collisionPair.second);
				if (collisionPair.first->DetectCollision(collision, boxCollider))
				{
					collisionVector.push_back(collision);
				}
				else delete collision;
			}
		}


		// 衝突応答
		for (Collision2D* collision : collisionVector)
		{
			Rigidbody2D* rigidbody = collision->collider->GetComponent<Rigidbody2D>();
			Rigidbody2D* rigidbody_Other = collision->otherCollider->GetComponent<Rigidbody2D>();

			Vector2 relativeVelocity = rigidbody->velocity - rigidbody_Other->velocity;

			//Debug::Log(L"相対速度： (%f, %f)", relativeVelocity.x, relativeVelocity.y);

			float forum1 = 1 / rigidbody->mass;

			for (auto collisionData : collision->collisionDataVector)
			{
				int collisionDataNum = collision->collisionDataVector.size();
				Vector2 impulse = -collision->NormalVector * (1 + e) / forum1 * min(Vector2::Dot(relativeVelocity, collision->NormalVector), 0) / collisionDataNum;

				// 重力キャンセル
				float gravityCancelScaler = Vector2::Dot(Vector2(0, 1) * rigidbody->mass * g * Time::FixedDeltaTime / collisionDataNum, collision->NormalVector);
				Vector2 gravityCancelImpulse = (-collision->NormalVector * min(Vector2::Dot(relativeVelocity, collision->NormalVector), 0)).Normalized() * gravityCancelScaler;
				impulse -= gravityCancelImpulse;

				// 速度反転が起きないなら速度を０にする撃力を与える
				if (impulse.magnitude < (relativeVelocity * rigidbody->mass).magnitude / collisionDataNum)
				{
					impulse = -collision->NormalVector * min(Vector2::Dot(relativeVelocity, collision->NormalVector), 0) * rigidbody->mass / collisionDataNum;
				}

				//Debug::Log(L"撃力： (%f, %f)", impulse.x, impulse.y);

				rigidbody->AddImpulse(impulse);
			}
		}

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
	//monoBehaviour->OnCollisionEnter2D(new Collision2D(FindGameObjectWith(body_B)->GetComponent<Collider2D>()));
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