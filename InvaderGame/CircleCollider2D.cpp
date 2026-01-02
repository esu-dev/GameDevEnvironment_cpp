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
	relativePosition = (-boxTransform->rotation.Get() * relativePosition.ToVector3()).ToVector2();
	Vector2 relativePosition_Abs = Vector2(fabsf(relativePosition.x), fabsf(relativePosition.y));
	Vector2 positionOnBoxCoordinate = relativePosition + boxTransform->position.Get().ToVector2();

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
	Vector2 closestPoint = collider->GetClosestPoint(positionOnBoxCoordinate);
	float distance = Vector2::Distance(positionOnBoxCoordinate, closestPoint);

	/*Debug::Log(L"相対位置： (%f, %f)", relativePosition.x, relativePosition.y);
	Debug::Log(L"最近傍点： (%f, %f)", closestPoint.x, closestPoint.y);*/

	if (relativePosition_Abs.x > boxTransform->scale.x / 2 ||
		relativePosition_Abs.y > boxTransform->scale.y / 2)
	{
		collisionData->depth = radius - distance;
		outCollision->Normal = (positionOnBoxCoordinate - closestPoint).Normalized();
	}


	// 回転を解消
	outCollision->Normal = (boxTransform->rotation.Get() * outCollision->Normal.ToVector3()).ToVector2();

	//Debug::Log(L"衝突法線： (%f, %f)", outCollision->NormalVector.x, outCollision->NormalVector.y);

	// 衝突点
	collisionData->contact = positionOnBoxCoordinate - outCollision->Normal * radius;

	return true;
}
