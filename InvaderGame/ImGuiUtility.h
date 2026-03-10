#pragma once

#include "EngineFramework.h"

class ImGuiUtility
{
public:
	static void Initialize(HWND hWnd);
	static void BeginFrame();
	static void Render();
};

