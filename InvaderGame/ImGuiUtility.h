#pragma once

#include "framework.h"

class ImGuiUtility
{
public:
	static void Initialize(HWND hWnd);
	static void BeginFrame();
	static void Render();
};

