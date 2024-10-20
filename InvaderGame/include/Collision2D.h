#pragma once

class Collider2D;

class Collision2D
{
public:
	Collision2D(Collider2D* collider);
	Collider2D* GetCollider();

private:
	Collider2D* _collider;
};