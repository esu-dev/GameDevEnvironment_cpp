#include "Collision2D.h"

Collision2D::Collision2D(Collider2D* collider) : _collider(collider)
{

}

Collider2D* Collision2D::GetCollider()
{
	return _collider;
}