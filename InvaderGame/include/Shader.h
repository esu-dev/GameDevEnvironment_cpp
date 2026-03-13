#pragma once

#include "DirectX.h"

class Shader
{
public:
	enum ShaderType
	{
		Normal2D,
		Instanced2D,
		Instanced3D
	};

	Shader(LPCWSTR shaderPath, LPCSTR vsFuncName, LPCSTR psFuncName);
	const ComPtr<ID3D11VertexShader> GetVertexShader();
	const ComPtr<ID3D11PixelShader> GetPixelShader();
	const ComPtr<ID3D11InputLayout> GetInputLayout();
	const ComPtr<ID3D11InputLayout> GetInputLayout_Batch();
	const ComPtr<ID3D11InputLayout> GetInputLayout3D();
	void CreateShader(ComPtr<ID3D11Device>& device, ShaderType shaderType);

private:
	LPCWSTR _shaderPath;
	LPCSTR _vsFuncName;
	LPCSTR _psFuncName;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11InputLayout> _inputLayout_Batch; // 2D用
	ComPtr<ID3D11InputLayout> _inputLayout3D;
};