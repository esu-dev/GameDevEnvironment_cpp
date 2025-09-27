#include "Collision2D.h"

Collision2D::Collision2D(Collider2D* collider, Collider2D* otherCollider) : collider(collider), otherCollider(otherCollider)
{

}

Collider2D* Collision2D::GetCollider()
{
	return collider;
}