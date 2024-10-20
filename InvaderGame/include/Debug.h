#pragma once

#include <stdarg.h>
#include "framework.h"

class Debug
{
public:
	static void Log(LPCWSTR log, ...)
	{
		wchar_t buffer[256];

		va_list args;
		va_start(args, log);
		vswprintf_s(buffer, 256, log, args);
		va_end(args);

		OutputDebugString(L"<Debug Log>\n");
		OutputDebugStringW(buffer);
		OutputDebugStringW(L"\n\n");
	}
};

