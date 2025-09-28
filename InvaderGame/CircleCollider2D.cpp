#include "CircleCollider2D.h"

#include "GameEngine.h"

void CircleCollider2D::Update()
{
	Collider2D::Update();
}

bool CircleCollider2D::DetectCollision(Collision2D* outCollision, BoxCollider2D* collider)
{
	float radius = this->GetTransform()->scale.x / 2;
	Transform* boxTransform = collider->gameObject->GetTransform();

	// 矩形からの相対座標を求める
	Vector2 relativePosition = (this->gameObject->GetTransform()->position - boxTransform->position).ToVector2();

	// 回転を考慮
	relativePosition = (-boxTransform->rotation * relativePosition.ToVector3()).ToVector2();
	Vector2 relativePosition_Abs = Vector2(fabsf(relativePosition.x), fabsf(relativePosition.y));

	// 衝突していなければ終了
	if (relativePosition_Abs.x > boxTransform->scale.x / 2 + radius ||
		relativePosition_Abs.y > boxTransform->scale.y / 2 + radius)
	{
		return false;
	}
	if (relativePosition_Abs.x > boxTransform->scale.x / 2 &&
		relativePosition_Abs.y > boxTransform->scale.y / 2)
	{
		if (Vector2::Distance(boxTransform->scale.ToVector2() / 2, relativePosition_Abs) > radius)
		{
			return false;
		}
	}


	Collision2D::CollisionData* collisionData = new Collision2D::CollisionData();
	outCollision->collisionDataVector.push_back(collisionData);

	// 衝突点の計算
	Vector2 collisionPointLocal;
	Vector2 closestPoint = collider->GetClosestPoint(relativePosition);
	float distance = Vector2::Distance(relativePosition, closestPoint);

	//Debug::Log(L"最近傍点： (%f, %f)", closestPoint.x, closestPoint.y);

	if (relativePosition_Abs.x > boxTransform->scale.x / 2 ||
		relativePosition_Abs.y > boxTransform->scale.y / 2)
	{
		collisionData->depth = radius - distance;
		outCollision->NormalVector = (relativePosition - closestPoint).Normalized();
	}
	else
	{

	}


	// 回転を解消
	collisionPointLocal = (boxTransform->rotation * collisionPointLocal.ToVector3()).ToVector2();
	outCollision->NormalVector = (boxTransform->rotation * outCollision->NormalVector.ToVector3()).ToVector2();

	//Debug::Log(L"衝突法線： (%f, %f)", outCollision->NormalVector.x, outCollision->NormalVector.y);

	// 衝突点
	collisionData->contact = boxTransform->position.ToVector2() + collisionPointLocal;

	return true;
}
