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

const ComPtr<ID3D11InputLayout> Shader::GetInputLayout3D()
{
	return _inputLayout3D;
}

void Shader::CreateShader(ComPtr<ID3D11Device>& device, ShaderType shaderType)
{
	ComPtr<ID3DBlob> compiledVS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, _vsFuncName, "vs_5_0", 0, 0, &compiledVS, nullptr);

	ComPtr<ID3DBlob> compiledPS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, _psFuncName, "ps_5_0", 0, 0, &compiledPS, nullptr);


	device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &_vertexShader);
	device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_pixelShader);


	if (shaderType == ShaderType::Normal2D)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXUV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		if (FAILED(device->CreateInputLayout(&layout[0], (UINT)layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout)))
		{
			MessageBoxW(NULL, L"Input Layoutを作成できませんでした．", L"エラー", MB_OK);
			return;
		}
	}
	else if (shaderType == ShaderType::Instanced2D)
	{
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

		if (FAILED(device->CreateInputLayout(&layout_Batch[0], (UINT)layout_Batch.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout_Batch)))
		{
			MessageBoxW(NULL, L"Input Layout(Batch 2D)を作成できませんでした．", L"エラー", MB_OK);
			return;
		}
	}
	else if (shaderType == ShaderType::Instanced3D)
	{
		// Inpute Layoutの作成
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout3D = {
			// --- スロット0: モデルの形状データ (VertexType2D) ---
			{ "POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			// --- スロット1: インスタンスデータ (InstanceBuffer) ---
			// 行列 (XMMATRIX) は float4 x 4回分として定義する
			{ "INST_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INST_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INST_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INST_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

			// 色 (XMFLOAT4)
			{ "INST_COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		if (FAILED(device->CreateInputLayout(&layout3D[0], (UINT)layout3D.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout3D)))
		{
			MessageBoxW(NULL, L"Input Layout(3D)を作成できませんでした．", L"エラー", MB_OK);
			return;
		}
	}
	else
	{
		MessageBoxW(NULL, L"ShaderTypeが定義されていません．", L"エラー", MB_OK);
		return;
	}
}