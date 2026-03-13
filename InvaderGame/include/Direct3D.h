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

// 3D用頂点構造体
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

struct ObjectData3D
{
	DirectX::XMMATRIX matrix;
	DirectX::XMFLOAT4 color;
};


class Direct3D
{
public:
	struct MeshData
	{
		std::vector<VertexType3D> vertexVec;
		std::vector<unsigned int> _indexVec;
	};

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext>	m_deviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_backBufferView;

	// 深度バッファ作成用（3D）
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;


	// シェーダー
	// 2D描画用
	Shader* _texShader;
	Shader* _colorShader;

	// 3D描画用
	Shader* _meshShader;


	bool Initialize(HWND hWnd, int width, int height);


	// 描画モードの初期化
	void InitMode2D();
	void InitMode3D();

	// カメラ情報の転送
	void SetCamMat2D(DirectX::XMVECTOR cameraPos, float size);
	void SetCamMat3D(DirectX::XMVECTOR cameraPos);

	// 情報のセット（2D）
	void SetInstanceData(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, Quaternion rotation, DirectX::XMFLOAT4 color, bool isFlipX);

	void SetMeshData(const std::vector<VertexType3D>& vertexVec, const std::vector<unsigned int>& indexVec);
	void SetObjectData(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Quaternion rotation, DirectX::XMFLOAT4 color);

	// メッシュデータの追加
	void AddMeshData(const std::vector<VertexType3D>& vertexVec);

	// 2D描画
	void Draw2D_Batch();
	void Draw2D_Batch(const Texture* texture);
	void DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView);

	// 3D描画
	void Draw3D();


private:
	static inline Direct3D* s_instance;


	// バッファ
	// 2D
	ComPtr<ID3D11Buffer> _quadVertexBuffer; // 固定頂点バッファ
	ComPtr<ID3D11Buffer> _cameraBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer; // インデックスバッファ
	ComPtr<ID3D11Buffer> _instanceBuffer; // インスタンスバッファ

	// 3D
	ComPtr<ID3D11Buffer> _camBuf3D;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _modelBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer3D;

	MeshData _meshData = MeshData();
	ObjectData3D _objectData;

	// 同じテクスチャをまとめてGPUに送るためのリスト
	std::vector<InstanceBuffer> _instBufVec;
	
	// モデルごとの頂点データのリスト
	std::vector<std::vector<VertexType3D>> _vertexVec;

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