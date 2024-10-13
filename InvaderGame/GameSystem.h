#pragma once

#define GAMESYS GameSystem::GetInstance()

#include "framework.h"
#include "GameEngine.h"

class GameSystem
{
public:
	static const int WINDOW_WIDTH = 960;
	static const int WINDOW_HEIGHT = 540;

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

	Texture m_Texture;
	Event OnUpdateListener;
	Event _delayedExecutionEvent;

	void AddDelayedExecution(std::function<void()> func);
	void Initialize();
	void Execute();

private:
	static inline GameSystem* s_instance;
	GameObject* _testObject;
	GameObject* BGM;
};