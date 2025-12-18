#pragma once

#include "framework.h"

class Physics2D
{
public:
	enum LibraryType
	{
		Box2D,
		Original
	};

	static const float GRAVITATIONAL_ACCELERATION;

	static b2Vec2 _gravity;
	static b2World _world;

	static b2World* GetBox2DWorld()
	{
		return &_world;
	}
	static LibraryType GetLibraryType();

	static void Initialize();
	static void Update();


private:
	static LibraryType _libraryType;
};

class MyContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);


private:
	std::function<void(void)> OnCollisionEnter;
};
