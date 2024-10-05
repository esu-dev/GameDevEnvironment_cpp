#include "Shader.h"

Shader::Shader(LPCWSTR shaderPath, LPCSTR vsFuncName, LPCSTR psFuncName)
	: _shaderPath(shaderPath), _vsFuncName(vsFuncName), _psFuncName(psFuncName) {}

const ComPtr<ID3D11VertexShader> Shader::GetVertexShader()
{
	return _vertexShader;
}

const ComPtr<ID3D11PixelShader> Shader::GetPixelShader()
{
	return _pixelShader;
}

const ComPtr<ID3D11InputLayout> Shader::GetInputLayout()
{
	return _inputLayout;
}

void Shader::CreateShader(ID3D11Device& device)
{
	ComPtr<ID3DBlob> compiledVS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, _vsFuncName, "vs_5_0", 0, 0, &compiledVS, nullptr);

	ComPtr<ID3DBlob> compiledPS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, _psFuncName, "ps_5_0", 0, 0, &compiledPS, nullptr);


	device.CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &_vertexShader);
	device.CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_pixelShader);


	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXUV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device.CreateInputLayout(&layout[0], layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout);
}