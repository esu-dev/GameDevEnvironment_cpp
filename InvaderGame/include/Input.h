#pragma once

#include "EngineFramework.h"

class Input
{
public:
	static void Update();
	static bool GetKey(int vkey);
	static bool GetKeyDown(int vkey);
	static bool AnyKeyDown();

private:
	static bool _isVkeyDown[150];
};
