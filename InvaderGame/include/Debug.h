#pragma once

#include <stdarg.h>
#include "EngineFramework.h"
//#include "DirectX.h"
#include <DirectXMath.h>

//#include "Vector3.h"
class Vector3;

class Debug
{
public:
	static void Log(LPCWSTR log, ...);
	static void Log(const char* log, ...);
	static void DrawLine(const Vector3& start, const Vector3& end, const DirectX::XMFLOAT4& color = {1, 1, 1, 1});
};

