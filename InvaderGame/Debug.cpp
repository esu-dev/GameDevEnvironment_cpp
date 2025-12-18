#include "Debug.h"

#include "Direct3D.h"
#include "Vector3.h"

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

void Debug::DrawLine(const Vector3& start, const Vector3& end, const DirectX::XMFLOAT4& color)
{
	Direct3D::GetInstance().SetColor(color);

	Vector3 v = end - start;
	Vector3 position = start + v / 2;
	float length = v.GetMagnitude();

	Direct3D::GetInstance().SetRect(position.x, position.y, length, 0.25f, Quaternion::FromToRotation(Vector3::right, v));
	Direct3D::GetInstance().Draw2D();
	Log("•`‰æ");
}