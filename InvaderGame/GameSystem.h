#pragma once

#define GAMESYS GameSystem::GetInstance()

#include "framework.h"
#include "GameObject.h"

class GameSystem
{
public:
	void Initialize();
	void Execute();
	void AddSquare(float x, float y, float w, float h);

	int WINDOW_WIDTH = 960;
	int WINDOW_HEIGHT = 540;
	Texture m_Texture;

private:
	static inline GameSystem* s_instance;

	std::vector<GameObject*> _gameObjectVector;

	GameSystem() { }

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
};