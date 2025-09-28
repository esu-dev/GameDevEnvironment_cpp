#include "BoxCollider2D.h"

#include "GameEngine.h"

BoxCollider2D::BoxCollider2D()
{
	_bodyDef.type = b2BodyType::b2_dynamicBody;
	_bodyDef.position.Set(0, 0);
	_body = Physics2D::GetBox2DWorld()->CreateBody(&_bodyDef);

	SetSize(Vector2(1.0f, 1.0f));
}

BoxCollider2D::~BoxCollider2D()
{
	Physics2D::GetBox2DWorld()->DestroyBody(_body);

	Debug::Log(L"BoxCollider2D Destractor");
}

void BoxCollider2D::SetSize(Vector2 size)
{
	_size = size;

	Vector2 v = Camera::WorldToBox2DWorld(size.ToVector3() * 0.5f);
	dynamicBox.SetAsBox(v.x, v.y);
}

void BoxCollider2D::SynchronizeBodyWithGameObject()
{
	Vector2 colliderPos = Camera::WorldToBox2DWorld(this->gameObject->GetTransform()->position) + _offset;
	_body->SetTransform(b2Vec2{ colliderPos.x, colliderPos.y }, _body->GetAngle());

	//Debug::Log(L"pos: %f, colliderPos: %f", this->gameObject->GetTransform()->position.y, colliderPos.y);
}

void BoxCollider2D::OnEnable()
{
	GAMESYS.AddDelayedExecution([&]() { _body->SetEnabled(true); });
}

void BoxCollider2D::OnDisable()
{
	GAMESYS.AddDelayedExecution([&]() { _body->SetEnabled(false); });
}

void BoxCollider2D::Start()
{
	SynchronizeBodyWithGameObject();
}

void BoxCollider2D::Update()
{
	Collider2D::Update();

	// b2Bodyの座標をオブジェクトと同期させる
	SynchronizeBodyWithGameObject();

	//Debug::Log(L"%f", _body->GetPosition().y);

	//Debug::Log(L"%f, %f, %f, %f", _body->GetPosition().x, _body->GetPosition().y, this->gameObject->transform->position.x, this->gameObject->transform->position.x * Camera::Magnification / _body->GetPosition().x);
}

Vector2 BoxCollider2D::GetClosestPoint(Vector2 point)
{
	float maxDistance = INFINITY;
	Vector2 closestPoint;

	Vector2 vertices[4];
	this->GetOBBvertices(vertices);

	for (int i = 0; i < 4; i++)
	{
		Vector2 A = vertices[i];
		Vector2 B = vertices[(i + 1) >= 4 ? 0 : (i + 1)];

		Vector2 minPoint = MathUtility::CalcMinDisPointOnLine(A, B, point);
		float distance = Vector2::Distance(minPoint, point);
		if (distance < maxDistance)
		{
			maxDistance = distance;
			closestPoint = minPoint;
		}
	}

	return closestPoint;
}

bool BoxCollider2D::DetectCollision(Collision2D* outCollision, BoxCollider2D* collider)
{
	Vector2 thisVertices[4];
	this->GetOBBvertices(thisVertices);

	for (int i = 0; i < 4; i++)
	{
		if (collider->IsPointIn(thisVertices[i]))
		{
			Vector2 closestPoint = collider->GetClosestPointByVertex(thisVertices, i);
			Collision2D::CollisionData* collisionData = new Collision2D::CollisionData();
			outCollision->collisionDataVector.push_back(collisionData);

			collisionData->depth = Vector2::Distance(thisVertices[i], closestPoint);
			collisionData->contact = thisVertices[i];
			outCollision->Normal = (closestPoint - thisVertices[i]).Normalized();
		}
	}

	return outCollision->collisionDataVector.size() > 0;
}

Vector2 BoxCollider2D::GetClosestPointByVertex(const Vector2 vertices[4], int targetIndex)
{
	Vector2 thisVertices[4];
	this->GetOBBvertices(thisVertices);

	Vector2 S = vertices[targetIndex];
	Vector2 T[2] = {
		vertices[(targetIndex - 1) < 0 ? 3 : (targetIndex - 1)],
		vertices[(targetIndex + 1) > 3 ? 0 : (targetIndex + 1)]
	};

	std::pair<Vector2, Vector2> lines[2];
	bool complete[2] = { false, false };

	for (int i = 0; i < 4; i++)
	{
		Vector2 O = thisVertices[i];
		Vector2 P = thisVertices[(i + 1) >= 4 ? 0 : (i + 1)];

		Vector2 crossPoint;
		for (int j = 0; j < 2; j++)
		{
			if (!complete[j] && MathUtility::CalcLineCrossPoint(&crossPoint, S, T[j], O, P))
			{
				lines[j] = { O, P };
				complete[j] = true;
			}
		}

		if (complete[0] && complete[1]) break;
	}

	Vector2 closestPoints[2];
	float distances[2];
	for (int j = 0; j < 2; j++)
	{
		closestPoints[j] = MathUtility::CalcMinDisPointOnLine(lines[j].first, lines[j].second, S);
		distances[j] = Vector2::Distance(S, closestPoints[j]);
	}

	if (complete[0] && complete[1])
	{
		if (distances[0] < distances[1]) return closestPoints[0];
		else return closestPoints[1];
	}

	for (int j = 0; j < 2; j++)
	{
		if (complete[j])
		{
			return closestPoints[j];
		}
	}
}

bool BoxCollider2D::IsPointIn(Vector2& point)
{
	Vector2 vertices[4];
	this->GetOBBvertices(vertices);

	for (int i = 0; i < 4; i++)
	{
		Vector2 A = vertices[i];
		Vector2 B = vertices[(i + 1) >= 4 ? 0 : (i + 1)];

		Vector2 AB = B - A;
		Vector2 AP = point - A;

		if (Vector2::Cross(AP, AB) < 0)
		{
			return false;
		}
	}

	return true;
}