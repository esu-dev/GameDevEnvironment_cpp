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

const ComPtr<ID3D11InputLayout> Shader::GetInputLayout_Batch()
{
	return _inputLayout_Batch;
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

	device.CreateInputLayout(&layout[0], (UINT)layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout);


	std::vector<D3D11_INPUT_ELEMENT_DESC> layout_Batch = {
		// --- スロット0: モデルの形状データ (VertexType2D) ---
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXUV",    0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// --- スロット1: インスタンスデータ (InstanceBuffer) ---
		// 行列 (XMMATRIX) は float4 x 4回分として定義する
		{ "INST_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INST_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INST_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INST_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		// 色 (XMFLOAT4)
		{ "INST_COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		// 反転フラグ (float)
		{ "INST_FLIPX",  0, DXGI_FORMAT_R32_FLOAT,          1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	device.CreateInputLayout(&layout_Batch[0], (UINT)layout_Batch.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout_Batch);
}