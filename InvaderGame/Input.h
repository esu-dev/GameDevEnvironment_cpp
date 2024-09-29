#pragma once

#include "framework.h"

class Input
{
public:
	static bool GetKey(int vkey)
	{
		return GetAsyncKeyState(vkey) & 0x8000;
	}
};
