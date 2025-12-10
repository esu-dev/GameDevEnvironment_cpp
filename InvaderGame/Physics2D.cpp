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
	if (EngineTime::TimeScale <= 0) return;

	if (_libraryType == LibraryType::Original)
	{
		static float g = 9.81f;
		static float e = 0.5f;

		auto gameObjectVector = SceneManager::GetActiveScene()->GetGameObjectVector();

		// 重力の適用
		for (GameObject* go : gameObjectVector)
		{
			if (Rigidbody2D* rigidbody = go->GetComponent<Rigidbody2D>())
			{
				if (rigidbody->IsKinematic) continue;

				rigidbody->velocity += Vector2(0, -g) * EngineTime::GetFixedDeltaTime();
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
					//Debug::Log(L"衝突検出（ブロードフェーズ）");
				}
			}
		}


		std::vector<Collision2D*> collisionVector;

		// 衝突検出（ナローフェーズ）
		for (auto collisionPair : collisionPairVector)
		{
			Collision2D* collision = new Collision2D(collisionPair.first, collisionPair.second);

			std::function<bool(Collider2D*, Collider2D*)> detectCollision = [&](Collider2D* colliderA, Collider2D* colliderB) -> bool {
					if (BoxCollider2D* boxCollider = dynamic_cast<BoxCollider2D*>(colliderB))
					{
						if (colliderA->DetectCollision(collision, boxCollider))
						{
							collisionVector.push_back(collision);
							return true;
							Debug::Log(L"衝突検出（ナローフェーズ）");
						}
					}
					return false;
				};

			if (detectCollision(collisionPair.first, collisionPair.second) ||
				detectCollision(collisionPair.second, collisionPair.first)) continue;

			delete collision;
		}


		// 衝突応答
		for (Collision2D* collision : collisionVector)
		{
			Vector2 sumImpulse;

			Rigidbody2D* rigidbody = collision->collider->GetComponent<Rigidbody2D>();
			Rigidbody2D* rigidbody_Other = collision->otherCollider->GetComponent<Rigidbody2D>();

			Rigidbody2D* rigidbodyA = rigidbody;
			Rigidbody2D* rigidbodyB = rigidbody_Other;

			// 自身がキネマティックなら交換
			if (rigidbody->IsKinematic)
			{
				rigidbodyA = rigidbody_Other;
				rigidbodyB = rigidbody;
				//collision->Normal *= -1;
			}

			bool isKinematic = false;
			if (rigidbody->IsKinematic || rigidbody_Other->IsKinematic) isKinematic = true;


			Vector2 relativeVelocity = rigidbodyA->velocity - rigidbodyB->velocity;
			//Debug::Log(L"相対速度： (%f, %f)", relativeVelocity.x, relativeVelocity.y);

			float forum1 = 1 / rigidbodyA->mass;
			if (isKinematic)
			{
				forum1 = 1 / rigidbodyA->mass;
			}
			else
			{
				forum1 = 1 / rigidbodyA->mass + 1 / rigidbodyB->mass;
			}

			int collisionDataNum = collision->collisionDataVector.size();

			for (auto collisionData : collision->collisionDataVector)
			{
				// 法線方向の相対速度
				float relNormalSpeed = min(Vector2::Dot(relativeVelocity, collision->Normal), 0);

				Vector2 impulse = -collision->Normal * (1 + e) / forum1 * relNormalSpeed / collisionDataNum;

				// 重力キャンセル
				if (isKinematic)
				{
					float gravityCancelScaler = Vector2::Dot(Vector2(0, 1) * rigidbodyA->mass * g * EngineTime::GetFixedDeltaTime() / collisionDataNum, collision->Normal);
					Vector2 gravityCancelImpulse = (-collision->Normal * relNormalSpeed).Normalized() * gravityCancelScaler;
					impulse -= gravityCancelImpulse;
				}

				// 速度反転が起きないなら速度を０にする撃力を与える
				if (isKinematic && impulse.magnitude < (relativeVelocity * rigidbodyA->mass).magnitude / collisionDataNum)
				{
					impulse = -collision->Normal * relNormalSpeed * rigidbodyA->mass / collisionDataNum;
				}

				// めり込み補正
				if (isKinematic && collisionData->depth > 0)
				{
					float power = rigidbodyA->mass * 10 * collisionData->depth;

					float down = relNormalSpeed * 5 * EngineTime::GetDelataTime();
					if (power + down < 0)
					{
						down = -power;
					}

					impulse += (collision->Normal * (power)) / collisionDataNum;
				}

				//Debug::Log(L"撃力： (%f, %f)", impulse.x, impulse.y);

				sumImpulse += impulse;
				rigidbodyA->AddImpulse(impulse); // rbにストックさせて、まとめて適用にするかも
				if (!isKinematic) rigidbodyB->AddImpulse(-impulse);
			}

			//Debug::Log(L"衝突法線： (%f, %f)", collision->Normal.x, collision->Normal.y);


			// 摩擦力
			Vector2 collisionLineVector = Vector2(-collision->Normal.y, collision->Normal.x);
			Vector2 collisionLineVelocity = collisionLineVector * Vector2::Dot(collisionLineVector, rigidbodyA->velocity);
			Vector2 direction = -collisionLineVelocity.Normalized();

			float mu = 0.1f;
			Vector2 friction = direction * mu * (sumImpulse / collisionDataNum).magnitude / EngineTime::GetFixedDeltaTime();
			Vector2 maxForce = collisionLineVelocity * rigidbodyA->mass / EngineTime::GetFixedDeltaTime();
			if (friction.magnitude > maxForce.magnitude)
			{
				friction = -maxForce;
			}
			rigidbodyA->AddImpulse(friction * EngineTime::GetFixedDeltaTime());
		}

		return;
	}

	float timeStep = EngineTime::GetFixedDeltaTime();
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