#include "Physics2D.h"

#include "GameEngine.h"
#include "Profiler.h"
#include "EngineBehaviour.h"

using namespace SceneManagement;

const float Physics2D::GRAVITATIONAL_ACCELERATION = 9.81f;


void Physics2D::Initialize()
{
}

void Physics2D::Update()
{
	if (EngineTime::TimeScale <= 0) return;

	auto* categoryData = Profiler::GetLastFrameData()->categories["Physics"];
	auto* subCD_broadPhase = new Profiler::FrameData();
	subCD_broadPhase->totalTime = 0.0f;
	categoryData->categories["BroadPhase"] = subCD_broadPhase;
	auto* subSubCD_AABB = new Profiler::FrameData();
	subSubCD_AABB->totalTime = 0.0f;
	subCD_broadPhase->categories["AABB"] = subSubCD_AABB;
	auto* subCD_narrowPhase = new Profiler::FrameData();
	subCD_narrowPhase->totalTime = 0.0f;
	categoryData->categories["NarrowPhase"] = subCD_narrowPhase;

	// deltaTimeがまれに０になることがある
	// 高fpsのときにおこる
	if (EngineTime::GetDeltaTime() == 0) return;

	static float g = GRAVITATIONAL_ACCELERATION;
	static float e = 0.0f;
	static float mu = 0.75f;

	auto gameObjectVector = SceneManager::GetActiveScene()->GetGameObjectVector();

	// 重力の適用
	for (GameObject* go : gameObjectVector)
	{
		if (Rigidbody2D* rigidbody = go->GetComponent<Rigidbody2D>())
		{
			rigidbody->ApplyGravity();
		}
	}


	std::vector<std::pair<Collider2D*, Collider2D*>> collisionPairVector;
		
	
	// 衝突検出（ブロードフェーズ）
	auto st_broadPhase = std::chrono::high_resolution_clock::now();
	
	struct PhysicsData
	{
		Collider2D* collider;
		Rigidbody2D* rigidbody;
	};

	// 判定が必要なGameObjectを検索
	std::vector<PhysicsData> goVecToCheck;
	for (GameObject* gameObject : gameObjectVector)
	{
		// 非アクティブならスキップ
		if (gameObject->ActiveSelf() == false) continue;
		Collider2D* collider = gameObject->GetComponent<Collider2D>();
		if (collider == nullptr) continue;

		// enableならスキップ
		if (collider->enabled == false) continue;

		goVecToCheck.push_back({ collider, gameObject->GetComponent<Rigidbody2D>() });
	}

	// AABB同士の衝突判定
	auto st_AABB = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < goVecToCheck.size(); i++)
	{
		Collider2D* colliderA = goVecToCheck[i].collider;

		for (int j = i + 1; j < goVecToCheck.size(); j++)
		{
			Collider2D* colliderB = goVecToCheck[j].collider;

			// 両方キネマティックなら衝突判定しない
			if (goVecToCheck[i].rigidbody->IsKinematic && goVecToCheck[j].rigidbody->IsKinematic)
			{
				continue;
			}

			if (colliderA->IsAABB_Collided(colliderB))
			{
				collisionPairVector.push_back({ colliderA, colliderB });
				//Debug::Log(L"衝突検出（ブロードフェーズ）");
			}
		}
	}

	auto et_AABB = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> dur_AABB = et_AABB - st_AABB;
	subSubCD_AABB->totalTime = dur_AABB.count();

	auto et_broadPhase = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> dur_broadPhase = et_broadPhase - st_broadPhase;
	subCD_broadPhase->totalTime = dur_broadPhase.count();

	
	// 衝突検出（ナローフェーズ）
	auto st_narrowPhase = std::chrono::high_resolution_clock::now();

	std::vector<std::shared_ptr<Collision2D>> collisionVector;
	for (auto collisionPair : collisionPairVector)
	{

		std::function<bool(Collider2D*, Collider2D*)> detectCollision = [&](Collider2D* colliderA, Collider2D* colliderB) -> bool {
				if (BoxCollider2D* boxCollider = dynamic_cast<BoxCollider2D*>(colliderB))
				{
					Collision2D* collision = new Collision2D(collisionPair.first, collisionPair.second);
					if (colliderA->DetectCollision(collision, boxCollider))
					{
						collisionVector.push_back(std::shared_ptr<Collision2D>(collision));
						return true;
					}
					delete collision;
				}
				return false;
			};

		if (detectCollision(collisionPair.first, collisionPair.second))
		{
			continue;
		}
	}

	auto et_narrowPhase = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> dur_narrowPhase = et_narrowPhase - st_narrowPhase;
	subCD_narrowPhase->totalTime = dur_narrowPhase.count();


	// 衝突応答
	for (auto collision : collisionVector)
	{
		Vector2 sumImpulse;

		Rigidbody2D* rigidbody = collision->collider->GetComponent<Rigidbody2D>();
		Rigidbody2D* rigidbody_Other = collision->otherCollider->GetComponent<Rigidbody2D>();

		Rigidbody2D* rigidbodyA = rigidbody;
		Rigidbody2D* rigidbodyB = rigidbody_Other;

		// isTriggerなら衝突応答は無し
		if (rigidbodyA->IsTrigger || rigidbodyB->IsTrigger)
		{
			continue;
		}

		// 自身がキネマティックなら交換
		if (rigidbody->IsKinematic)
		{
			rigidbodyA = rigidbody_Other;
			rigidbodyB = rigidbody;
			collision->Normal *= -1;
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

		int collisionDataNum = (int)collision->collisionDataVector.size();

		for (auto collisionData : collision->collisionDataVector)
		{
			// 法線方向の相対速度
			float relNormalSpeed = min(Vector2::Dot(relativeVelocity, collision->Normal), 0);

			Vector2 impulse = -collision->Normal * (1 + e) / forum1 * relNormalSpeed / (float)collisionDataNum;

			// 重力キャンセル
			// これあるとぶっ飛ぶ
			//if (isKinematic)
			/*{
				float gravityCancelScaler = Vector2::Dot(Vector2(0, 1) * rigidbodyA->mass * g * EngineTime::GetFixedDeltaTime() / collisionDataNum, collision->Normal);
				Vector2 gravityCancelImpulse = (-collision->Normal * relNormalSpeed).Normalized() * gravityCancelScaler;
				impulse -= gravityCancelImpulse;
			}*/

			// 速度反転が起きないなら速度を０にする撃力を与える
			// 条件にimpulseを使っているためか挙動がおかしくなる
			if (impulse.GetMagnitude() < (relativeVelocity * rigidbodyA->mass).GetMagnitude() / collisionDataNum)
			{
				impulse = -collision->Normal * relNormalSpeed * rigidbodyA->mass / (float)collisionDataNum;
			}

			// めり込み補正
			if (collisionData->depth > 0)
			{
				float power = rigidbodyA->mass * 3 * collisionData->depth;

				float down = relNormalSpeed * 10 * EngineTime::GetDeltaTime();
				if (power + down < 0)
				{
					down = -power * 0.9f;
				}

				impulse += (collision->Normal * (power + down)) / (float)collisionDataNum;
			}

			//Debug::Log(L"撃力： (%f, %f)", impulse.x, impulse.y);

			sumImpulse += impulse;
			rigidbodyA->AddImpulse(impulse); // rbにストックさせて、まとめて適用にするかも
			if (!isKinematic) rigidbodyB->AddImpulse(-impulse);

			// デバッグ描画
			float minLength = 0.5f;
			float length = minLength;
			if (impulse.GetMagnitude() >= minLength)
			{
				length = 1 * log2(impulse.GetMagnitude());
			}
			//Debug::DrawLine(collisionData->contact.ToVector3(), (collisionData->contact + impulse.Normalized() * length).ToVector3(), DirectX::XMFLOAT4(0.5, 0.5, 0, 1));
		}

		//Debug::Log(L"衝突法線： (%f, %f)", collision->Normal.x, collision->Normal.y);


		// 摩擦力
		Vector2 collisionLineVector = Vector2(-collision->Normal.y, collision->Normal.x);
		Vector2 collisionLineVelocity = collisionLineVector * Vector2::Dot(collisionLineVector, rigidbodyA->velocity);
		Vector2 direction = -collisionLineVelocity.Normalized();

		Vector2 friction = direction * mu * (sumImpulse / (float)collisionDataNum).GetMagnitude() / EngineTime::GetFixedDeltaTime();
		Vector2 maxForce = collisionLineVelocity * rigidbodyA->mass / EngineTime::GetFixedDeltaTime();
		if (friction.GetMagnitude() > maxForce.GetMagnitude())
		{
			friction = -maxForce;
		}
		//Debug::Log("(%f, %f), %f", friction.x, friction.y, EngineTime::GetFixedDeltaTime());
		rigidbodyA->AddImpulse(friction * EngineTime::GetFixedDeltaTime());


		// イベント発火
		// for (auto& component : collision->collider->gameObject->GetComponentVector())
		// {
		// 	if (EngineBehaviour* engineBehaviour = dynamic_cast<EngineBehaviour*>(component.get()))
		// 	{
		// 		engineBehaviour->OnCollisionEnter2D(collision->otherCollider->gameObject);
		// 	}
		// }
		// for (auto& component : collision->otherCollider->gameObject->GetComponentVector())
		// {
		// 	if (EngineBehaviour* engineBehaviour = dynamic_cast<EngineBehaviour*>(component.get()))
		// 	{
		// 		engineBehaviour->OnCollisionEnter2D(collision->collider->gameObject);
		// 	}
		// }
	}

	// イベント発火
	for (auto collision : collisionVector)
	{
		Rigidbody2D* rigidbody = collision->collider->GetComponent<Rigidbody2D>();
		Rigidbody2D* rigidbody_Other = collision->otherCollider->GetComponent<Rigidbody2D>();

		auto callTriggerStay2D = [](Collider2D* collider, Collider2D* other) -> void {
			for (auto& component : collider->gameObject->GetComponentVector())
			{
				if (EngineBehaviour* engineBehaviour = dynamic_cast<EngineBehaviour*>(component.get()))
				{
					engineBehaviour->OnTriggerStay2D(other->gameObject);
				}
			}
		};

		if (rigidbody->IsTrigger)
		{
			callTriggerStay2D(collision->collider, collision->otherCollider);
		}
		if (rigidbody_Other->IsTrigger)
		{
			callTriggerStay2D(collision->otherCollider, collision->collider);
		}
	}
}
