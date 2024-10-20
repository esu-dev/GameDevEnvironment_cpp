#pragma once

#include "DirectX.h"

class Shader
{
public:
	Shader(LPCWSTR shaderPath, LPCSTR vsFuncName, LPCSTR psFuncName);
	const ComPtr<ID3D11VertexShader> GetVertexShader();
	const ComPtr<ID3D11PixelShader> GetPixelShader();
	const ComPtr<ID3D11InputLayout> GetInputLayout();
	void CreateShader(ID3D11Device& device);

private:
	LPCWSTR _shaderPath;
	LPCSTR _vsFuncName;
	LPCSTR _psFuncName;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
};