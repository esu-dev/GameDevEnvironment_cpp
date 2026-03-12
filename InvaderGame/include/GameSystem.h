#pragma once

#define GAMESYS GameSystem::GetInstance()

#include "EngineFramework.h"

#include "Texture.h"
#include "Event.h"

class GameSystem
{
public:
	static const int WINDOW_WIDTH = 1440;
	static const int WINDOW_HEIGHT = 810;

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
	void AddRenderingData(int order, std::function<void()> func);
	void AddRenderingFunc(std::function<void()> func);


private:
	static inline GameSystem* s_instance;
	struct RenderingData
	{
		int order = 0;
		std::function<void()> function;
	};

	// 2D用
	std::vector<RenderingData*> _renderingDataVector;

	// 3D用
	std::vector<std::function<void()>> _renderingFuncVec;
};