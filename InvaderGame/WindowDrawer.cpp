#include "WindowDrawer.h"

WindowDrawer::WindowDrawer(LPCWSTR shaderPath) : _shaderPath(shaderPath)
{
	//CreateShader();
}

void WindowDrawer::CreateShader()
{
	ComPtr<ID3DBlob> compiledVS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, "VS", "ps_5_0", 0, 0, &compiledVS, nullptr);

	ComPtr<ID3DBlob> compiledPS;
	D3DCompileFromFile(_shaderPath, nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr);


	_device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &_vertexShader);
	_device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_pixelShader);


}