#pragma once

#define GAMESYS GameSystem::GetInstance()

#include "framework.h"
#include "GameEngine.h"

class GameSystem
{
public:
public:
	static void CreateInstance()
	{
		DeleteInstance();
		s_instance = new GameSystem();
	}

	static void DeleteInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	static GameSystem& GetInstance()
	{
		return *s_instance;
	}

	int WINDOW_WIDTH = 960;
	int WINDOW_HEIGHT = 540;
	Texture m_Texture;
	Event OnUpdateListener;

	b2World* GetBox2DWorld();
	void Initialize();
	void Execute();
	void AddSquare(float x, float y, float w, float h);

private:
	static inline GameSystem* s_instance;
 	b2Vec2 _gravity = { 0.0f, -9.81f };
	b2World _world{_gravity};
	std::vector<GameObject*> _gameObjectVector;

	GameSystem() { }
};