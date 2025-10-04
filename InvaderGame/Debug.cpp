#include "Debug.h"

void Debug::Log(LPCWSTR log, ...)
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

void Debug::Log(const char* log, ...)
{
	char buffer[256];

	va_list args;
	va_start(args, log);
	vsprintf_s(buffer, 256, log, args);
	va_end(args);

	// char*‚ðwchar_t*‚É•ÏŠ·
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[256];
	mbstowcs_s(&convertedChars, wcstring, 256, buffer, _TRUNCATE);

	OutputDebugString(L"<Debug Log>\n");
	OutputDebugString(wcstring);
	OutputDebugStringW(L"\n\n");
}