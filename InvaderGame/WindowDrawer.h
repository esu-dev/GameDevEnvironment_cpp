#pragma once

#include "DirectX.h"
#include "framework.h"

class WindowDrawer
{
public:
	WindowDrawer(LPCWSTR shaderPath);

private:
	LPCWSTR _shaderPath;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;

	void CreateShader();
};