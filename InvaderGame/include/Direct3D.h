#pragma once

#include "DirectX.h"

#include "Vector3.h"
#include "Quaternion.h"

class Texture;
class Quaternion;
class Shader;

struct CameraBuffer
{
	DirectX::XMMATRIX viewProjMat;
};

// 2D用頂点構造体
struct VertexType2D
{
	DirectX::XMFLOAT3 Pos; // 座標
	DirectX::XMFLOAT2 UV; // UV座標
};

struct VertexType3D
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
};

struct InstanceBuffer
{
	DirectX::XMMATRIX matrix; // 48 byte
	DirectX::XMFLOAT4 color; // 16 byte
	float flipX; // 4 byte
	float padding[3]; // 12 byte
};


class Direct3D
{
public:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext>	m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_backBufferView;


	// 2D描画用のシェーダー
	Shader* _texShader_Batch;
	Shader* _colorShader_Batch;


	bool Initialize(HWND hWnd, int width, int height);


	// 2D描画モードにする
	void ChangeMode_2D();


	void StartRendering(DirectX::XMVECTOR cameraPos);

	// 情報のセット
	void SetInstanceData(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, Quaternion rotation, DirectX::XMFLOAT4 color, bool isFlipX);


	// 2D描画
	void Draw2D_Batch();
	void Draw2D_Batch(const Texture* texture);
	void DrawRect(const Vector2& center, const Vector2& size, const Quaternion& rotation, DirectX::XMFLOAT4 color);
	void DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView);

	// 3D描画
	void AddMeshData(const std::vector<VertexType3D>& vertexVec);
	void Draw();


private:
	static inline Direct3D* s_instance;

	// バッファ
	ComPtr<ID3D11Buffer> _quadVertexBuffer;
	ComPtr<ID3D11Buffer> _cameraBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	ComPtr<ID3D11Buffer> _colorBuffer;
	ComPtr<ID3D11Buffer> _instanceBuffer;

	ComPtr<ID3D11Buffer> _vertexBuffer;

	
	std::vector<InstanceBuffer> _instBufVec;
	
	std::vector<VertexType3D> _vertexVec;

	Direct3D();
	void SetGpuData();


public:
	static void CreateInstance()
	{
		DeleteInstance();

		s_instance = new Direct3D();
	}

	static void DeleteInstance()
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}

	static Direct3D& GetInstance()
	{
		return *s_instance;
	}
};

#define D3D Direct3D::GetInstance()