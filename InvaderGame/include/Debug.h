#pragma once

#include <stdarg.h>
#include "framework.h"

class Debug
{
public:
	static void Log(LPCWSTR log, ...);
	static void Log(const char* log, ...);
};

