#include "Collider2D.h"

#include "GameEngine.h"

void Collider2D::Update()
{
	if (!_showAABB) return;
	Transform* transform = this->gameObject->GetTransform();
	auto AABB = GetAABB();
	Vector2 size = AABB.second - AABB.first;
	Direct3D::GetInstance().DrawRect(transform->position.ToVector2(), size, transform->rotation, DirectX::XMFLOAT4(0, 0, 1, 1));
}

void Collider2D::SetOffset(Vector2 centerPos)
{
	_offset = centerPos;
}

b2Body* Collider2D::Getb2Body()
{
	return _body;
}

b2PolygonShape* Collider2D::Getb2PolygonShape()
{
	return &dynamicBox;
}

bool Collider2D::IsAABB_Collided(Collider2D* collider)
{
	auto AABB_a = this->GetAABB();
	auto AABB_b = collider->GetAABB();

	Vector2 minA = AABB_a.first;
	Vector2 maxA = AABB_a.second;

	Vector2 minB = AABB_b.first;
	Vector2 maxB = AABB_b.second;

	if (((minA.x >= minB.x && minA.x <= maxB.x) || (minB.x >= minA.x && minB.x <= maxA.x)) &&
		((minA.y >= minB.y && minA.y <= maxB.y) || (minB.y >= minA.y && minB.y <= maxA.y)))
	{
		return true;
	}

	return false;
}


// private
std::pair<Vector2, Vector2> Collider2D::GetAABB()
{
	//Debug::Log("%s", this->gameObject->tag);
	Transform* transform = this->gameObject->GetTransform();

	// ’¸“_‚ÌŽæ“¾
	Vector2 points[4];
	GetOBBvertices(points);

	// ’¸“_‚Ì‰ñ“]
	for (int i = 0; i < sizeof(points) / sizeof(Vector2); i++)
	{
		points[i] = (transform->rotation * (points[i].ToVector3() - transform->position) + transform->position).ToVector2();
	}

	// AABB‚ÌŒvŽZ
	Vector2 minPoint = Vector2(INFINITY, INFINITY);
	Vector2 maxPoint = Vector2(-INFINITY, -INFINITY);
	for (Vector2 &v : points)
	{
		if (v.x < minPoint.x)
		{
			minPoint.x = v.x;
		}
		else if (v.x > maxPoint.x)
		{
			maxPoint.x = v.x;
		}

		if (v.y < minPoint.y)
		{
			minPoint.y = v.y;
		}
		else if (v.y > maxPoint.y)
		{
			maxPoint.y = v.y;
		}
	}

	return { minPoint, maxPoint };
}

void Collider2D::GetOBBvertices(Vector2 outVertices[4])
{
	Transform* transform = this->GetTransform();
	outVertices[0] = Vector2(transform->position.x + transform->scale.x / 2, transform->position.y + transform->scale.y / 2);
	outVertices[1] = Vector2(transform->position.x + transform->scale.x / 2, transform->position.y - transform->scale.y / 2);
	outVertices[2] = Vector2(transform->position.x - transform->scale.x / 2, transform->position.y - transform->scale.y / 2);
	outVertices[3] = Vector2(transform->position.x - transform->scale.x / 2, transform->position.y + transform->scale.y / 2);
}