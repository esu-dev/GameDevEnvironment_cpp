#pragma once

#include "framework.h"
#include "GameObject.h"

class Component
{
public:
	GameObject* gameObject;

	//virtual void OnCollisionEnter() = 0;
	virtual void Start() {}
	virtual void Update() {}


//private:
//	class CollisionEvent : public b2ContactListener
//	{
//	public:
//		CollisionEvent();
//
//		void BeginContact(b2Contact* contact);
//		void EndContact(b2Contact* contact);
//		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
//		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
//
//
//	private:
//		std::function<void(void)> OnCollisionEnter;
//	};
//
//	CollisionEvent _collisionEvent;
};
