#pragma once

#include "framework.h"

class Input
{
public:
	static void Update();
	static bool GetKey(int vkey);
	static bool GetKeyDown(int vkey);

private:
	static bool _isVkeyDown[150];
};
