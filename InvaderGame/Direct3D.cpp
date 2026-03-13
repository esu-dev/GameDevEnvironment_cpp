// 参考　https://gamesgard.com/directx11_lesson02/

#include "EngineFramework.h"

#include "Direct3D.h"
#include "Shader.h"

#include "GameEngine.h"
#include "GameSystem.h"


Direct3D::Direct3D() :
	_texShader(new Shader(L"Shader/SpriteShader.hlsl", "VS_New", "PS_New")),
	_colorShader(new Shader(L"Shader/SpriteShader.hlsl", "VS_New", "PS_New_Color")),
	_meshShader(new Shader(L"Shader/ModelShader.hlsl", "VS_Main", "PS_Main")) {}

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
	//=====================================================
	// ファクトリー作成(ビデオ グラフィックの設定の列挙や指定に使用されるオブジェクト)
	//=====================================================
	ComPtr<IDXGIFactory> factory;

	// CreateDXGIFactory1(); DXGIファクトリの生成
	// IID_PPV_ARGSマクロ インターフェースポインタを取得するために使用される。２つの変数となる
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
	{
		return false;
	}

	//=====================================================
	//デバイス生成(主にリソース作成時に使用するオブジェクト)
	//=====================================================
	UINT creationFlags = 0;

#ifdef _DEBUG
	// デバッグレイヤーをサポートするデバイスを作成するフラグを立てられる。
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// FeatureLevelはドライバの機能レベルを指している。
	// ドライバはOSや周辺機器とPC等のハードを橋渡しし、動作させる役割を果たす。
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};

	// デバイスとデバイスコンテキストを作成
	D3D_FEATURE_LEVEL futureLevel;
	if (FAILED(D3D11CreateDevice(
			nullptr, // IDXGIAdapter*
			D3D_DRIVER_TYPE_HARDWARE, // D3D_DRIVER_TYPE
			nullptr, // HMODULE
			creationFlags, // UINT
			featureLevels, // D3D_FEATURE_LEVEL*
			_countof(featureLevels), // UINT
			D3D11_SDK_VERSION, // UINT
			&_device, // ID3D11Device**
			&futureLevel, // D3D_FEATURE_LEVEL*
			&m_deviceContext // ID3D11DeviceContext**
		)))
	{
		return false;
	}


	//=====================================================
	// スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC scDesc = {};		// スワップチェーンの設定データ
	scDesc.BufferDesc.Width = width;						// 画面の幅
	scDesc.BufferDesc.Height = height;						// 画面の高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バッファの形式。アルファを含むチャネルあたり 8 ビットをサポートする 4 コンポーネントの 32 ビット符号なし正規化整数形式。一般的な256だから分かりやすい
	//scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 逆ガンマ補正をかけるフォーマット。jpeg,pngはガンマ補正されて保存されている。
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // スキャンライン（走査線）の指定
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // ウィンドウ描画時のスケーリングの指定
	scDesc.BufferDesc.RefreshRate.Numerator = 0; // リフレッシュレートの分子
	scDesc.BufferDesc.RefreshRate.Denominator = 1; // リフレッシュレートの分母
	scDesc.SampleDesc.Count = 1;							// １ピクセルの色を決めるサンプリング数 マルチサンプリングは使用しない。アンチエイリアス処理時に、中間色補完の基準色をいくつのサンプルを元にするかの設定。重い。
	scDesc.SampleDesc.Quality = 0;							// マルチサンプリングの品質。使用しない
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使用方法
	scDesc.BufferCount = 2;									// スワップチェインのバッファの数
	scDesc.OutputWindow = hWnd; // 出力先ウィンドウハンドル
	scDesc.Windowed = TRUE;									// ウィンドウモード
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // スワップエフェクトの設定。ディスプレイ表示後のバックバッファの状態について？おすすめはDiscard
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // スワップチェインの設定フラグの設定。ウィンドウモードからフルスクリーンに切り替えると、アプリケーションウィンドウのサイズに合わせて解像度が変更される設定
	
	// スワップチェインの作成
	if (FAILED(factory->CreateSwapChain(_device.Get(), &scDesc, &m_swapChain)))
	{
		return false;
	}

	// スワップチェインからバックバッファリソース取得
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
	{
		return false;
	}

	// バックバッファリソース用のRTVを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = scDesc.BufferDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // リソースは 2D テクスチャとしてアクセスされます。
	if (FAILED(_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
	{
		return false;
	}


	//=====================================================
	// デバイスコンテキストに描画に関する設定を行っておく
	//=====================================================
	// バックバッファをRTとしてセット
	// ここでGetAddressOf()を使うのは、ComPtrでは&がオーバーロードされているから
	//m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

	//// ビューポートの設定
	//D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	//m_deviceContext->RSSetViewports(1, &vp);

	// 深度バッファの作成
	// 1. テクスチャ（2Dバッファ）の作成
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = GameSystem::WINDOW_WIDTH;                // ウィンドウ幅
	depthDesc.Height = GameSystem::WINDOW_HEIGHT;              // ウィンドウ高さ
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 一般的な24bit深度
	depthDesc.SampleDesc.Count = 1;         // マルチサンプルなし
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> depthBuffer;
	_device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf());

	// 2. 深度ステンシルビュー (DSV) の作成
	_device->CreateDepthStencilView(depthBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf());

	// 第3引数に NULL ではなく DSV を渡す
	m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), m_depthStencilView.Get());


	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);


	// Depth Stencil Stateのセット
	CD3D11_DEPTH_STENCIL_DESC dsDesc(D3D11_DEFAULT);
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // 手前にあるものを描く

	ComPtr<ID3D11DepthStencilState> pDSState;
	_device->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf());
	m_deviceContext->OMSetDepthStencilState(pDSState.Get(), 1);


	//=====================================================
	// シェーダーの作成
	//=====================================================
	// 2D
	_texShader->CreateShader(*_device.Get());
	_colorShader->CreateShader(*_device.Get());

	// 3D
	_meshShader->CreateShader(*_device.Get());


	return true;
}

void Direct3D::InitMode2D()
{
	// 固定頂点バッファの作成
	// 1. 頂点データ（四角形）の準備
	VertexType2D vertices[] = {
		{ {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f} }, // 左上
		{ { 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f} }, // 右上
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f} }, // 左下
		{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f} }, // 右下
	};

	// 2. バッファの設定（不変なバッファとして作成）
	D3D11_BUFFER_DESC vDesc = {};
	vDesc.Usage = D3D11_USAGE_IMMUTABLE; // 書き換えない
	vDesc.ByteWidth = sizeof(VertexType2D) * 4;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vData = {};
	vData.pSysMem = vertices;

	// 3. 生成
	_device->CreateBuffer(&vDesc, &vData, _quadVertexBuffer.GetAddressOf());


	// カメラ定数バッファの作成
	D3D11_BUFFER_DESC bufferDesk = {};
	bufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesk.ByteWidth = sizeof(CameraBuffer);
	bufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(_device->CreateBuffer(&bufferDesk, nullptr, _cameraBuffer.GetAddressOf())))
	{
		//MessageBox(NULL, L"定数バッファを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}


	// プロミティブ・トポロジーをセット
	// これもう使っていないかも？
	D3D.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形メッシュ


	// インスタンスバッファの作成
	D3D11_BUFFER_DESC instanceBufferDesk = {};
	instanceBufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesk.ByteWidth = sizeof(InstanceBuffer) * 1000; // 1000個の配列にできるということか？
	instanceBufferDesk.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesk.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	_device->CreateBuffer(&instanceBufferDesk, nullptr, _instanceBuffer.GetAddressOf());


	// インデックスバッファの作成
	// 四角形1つ分のインデックス
	unsigned short indices[] = { 0, 1, 2, 1, 3, 2 };

	CD3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufSRdata = {};
	indexBufSRdata.pSysMem = indices;
	_device->CreateBuffer(&indexBufferDesc, &indexBufSRdata, _indexBuffer.GetAddressOf());


	// サンプラーステートを作成しセットする
	{
		// 異方性フィルタリング補間、Wrapモード
		D3D11_SAMPLER_DESC desc = {};
		//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// 線形フィルタリング ドット絵には向かない設定
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// ポイントサンプリング
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// テクスチャアドレッシングモードをWrapに
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// テクスチャアドレッシングモードをWrapに
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// テクスチャアドレッシングモードをWrapに
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// ステートオブジェクト作成
		ComPtr<ID3D11SamplerState> state;
		_device->CreateSamplerState(&desc, &state);

		// 各シェーダーの0番目にセット(実際は必要なシェーダーだけセットしてください)
		m_deviceContext->VSSetSamplers(0, 1, state.GetAddressOf()); // 頂点シェーダーの0番目にセット
		m_deviceContext->PSSetSamplers(0, 1, state.GetAddressOf()); // ピクセルシェーダーの0番目にセット
		m_deviceContext->GSSetSamplers(0, 1, state.GetAddressOf()); // ジオメトリシェーダーの0番目にセット
		m_deviceContext->CSSetSamplers(0, 1, state.GetAddressOf()); // コンピュートシェーダーの0番目にセット
	}


	// ブレンドの設定
	D3D11_BLEND_DESC blendDesc; // ブレンド状態について設定する構造体
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Alpha値を全てのカラー要素に乗算する
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// ブレンドステートを作成
	ComPtr<ID3D11BlendState> blendState;
	if (FAILED(_device->CreateBlendState(&blendDesc, blendState.GetAddressOf())))
	{
		//MessageBox(NULL, L"ブレンドステートを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// ブレンドをセット
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	m_deviceContext->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
}

void Direct3D::InitMode3D()
{
	// ラスタライザの作成
	// もし２D描画に影響が出るなら、deviceContextを分離する必要があるかも
	CD3D11_RASTERIZER_DESC rsDesc(D3D11_DEFAULT);
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;

	ComPtr<ID3D11RasterizerState> pRS;
	_device->CreateRasterizerState(&rsDesc, pRS.GetAddressOf());

	m_deviceContext->RSSetState(pRS.Get());


	// カメラ定数バッファの作成
	{
		D3D11_BUFFER_DESC bufferDesk = {};
		bufferDesk.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesk.ByteWidth = sizeof(CameraBuffer);
		bufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (FAILED(_device->CreateBuffer(&bufferDesk, nullptr, _camBuf3D.GetAddressOf())))
		{
			MessageBoxW(NULL, L"３D用カメラ定数バッファを作成できませんでした．", L"エラー", MB_OK);
			return;
		}
	}


	// 頂点バッファの作成
	D3D11_BUFFER_DESC vDesc = {};
	vDesc.Usage = D3D11_USAGE_DYNAMIC;
	vDesc.ByteWidth = sizeof(VertexType3D) * 100000; // 10万頂点まで対応可能
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(_device->CreateBuffer(&vDesc, nullptr, _vertexBuffer.GetAddressOf())))
	{
		Debug::Log("_vertexBufferの作成に失敗しました．[Direct3D::InitMode3D()]");
	}


	// モデルバッファの作成
	{
		D3D11_BUFFER_DESC bufferDesk = {};
		bufferDesk.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesk.ByteWidth = sizeof(ObjectData3D);
		bufferDesk.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (FAILED(_device->CreateBuffer(&bufferDesk, nullptr, _modelBuffer.GetAddressOf())))
		{
			MessageBoxW(NULL, L"モデル頂点バッファを作成できませんでした．", L"エラー", MB_OK);
			return;
		}
	}


	// インデックスバッファの作成
	CD3D11_BUFFER_DESC indexBD = {}; // 0埋めは必須 // C++で追加されたConvenienceなヘルパークラス
	indexBD.Usage = D3D11_USAGE_DYNAMIC;
	indexBD.ByteWidth = sizeof(unsigned int) * 100000;
	indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(_device->CreateBuffer(&indexBD, nullptr, _indexBuffer3D.GetAddressOf())))
	{
		MessageBoxW(NULL, L"インデックスバッファを作成できませんでした．", L"エラー", MB_OK);
		return;
	}
}

void Direct3D::SetCamMat2D(DirectX::XMVECTOR cameraPos, float size)
{
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(cameraPos, DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorSet(0, 0, 1, 0)), ((Vector3)Vector3::up).ToXMVECTOR());
	DirectX::XMMATRIX projMat = DirectX::XMMatrixOrthographicLH(GameSystem::WINDOW_WIDTH / Camera::Magnification / size, GameSystem::WINDOW_HEIGHT / Camera::Magnification / size, 0.0f, 1000.0f);
	//DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), (float)GameSystem::WINDOW_WIDTH / (float)GameSystem::WINDOW_HEIGHT, 0.1f, 1000);

	// 定数バッファへの書き込み
	CameraBuffer constantBuffer;
	constantBuffer.viewProjMat = DirectX::XMMatrixTranspose(viewMat * projMat);
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	if (SUCCEEDED(m_deviceContext->Map(_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		memcpy(mappedSubresource.pData, &constantBuffer, sizeof(CameraBuffer));
		m_deviceContext->Unmap(_cameraBuffer.Get(), 0);
	}
	else
	{
		//MessageBox(NULL, L"定数バッファを設定できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// 頂点シェーダーに定数バッファを設定
	m_deviceContext->VSSetConstantBuffers(0, 1, _cameraBuffer.GetAddressOf());
}

void Direct3D::SetCamMat3D(DirectX::XMVECTOR cameraPos)
{
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(cameraPos, DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorSet(0, 0, 1, 0)), DirectX::XMVectorSet(0, 1, 0, 0));

	float aspect = (float)GameSystem::WINDOW_WIDTH / (float)GameSystem::WINDOW_HEIGHT;
	DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), aspect, 0.1f, 1000.0f);

	CameraBuffer cameraBuffer;
	cameraBuffer.viewProjMat = DirectX::XMMatrixTranspose(viewMat * projMat);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	if (SUCCEEDED(m_deviceContext->Map(_camBuf3D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		memcpy(mappedSubresource.pData, &cameraBuffer, sizeof(CameraBuffer));
		m_deviceContext->Unmap(_camBuf3D.Get(), 0);
	}
	else
	{
		//MessageBox(NULL, L"定数バッファを設定できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	m_deviceContext->VSSetConstantBuffers(1, 1, _camBuf3D.GetAddressOf()); // 第１引数はスロット指定
}


void Direct3D::SetInstanceData(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, Quaternion rotation, DirectX::XMFLOAT4 color, bool isFlipX)
{
	// スケールを行列化
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, 1);

	// 回転
	DirectX::XMVECTOR q = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(q);

	// 移動を行列化
	DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixTranslation(pos.x, pos.y, 0.0f);

	// ワールド行列の作成
	DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * transformMatrix;


	// インスタンスデータの作成
	InstanceBuffer instanceBuffer;
	instanceBuffer.matrix = DirectX::XMMatrixTranspose(worldMatrix); // 転置
	instanceBuffer.color = color;
	instanceBuffer.flipX = isFlipX ? 1.0f : 0.0f;


	// vectorに追加
	_instBufVec.push_back(instanceBuffer);
}

void Direct3D::SetMeshData(const std::vector<VertexType3D>& vertexVec, const std::vector<unsigned int>& indexVec)
{
	_meshData.vertexVec = vertexVec;
	_meshData._indexVec = indexVec;
}

void Direct3D::SetObjectData(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, Quaternion rotation, DirectX::XMFLOAT4 color)
{
	// スケールを行列化
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	// 回転
	DirectX::XMVECTOR q = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(q);

	// 移動を行列化
	DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	// ワールド行列の作成
	DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * transformMatrix;

	_objectData.matrix = worldMatrix;
	_objectData.color = color;
}

void Direct3D::Draw2D_Batch()
{
	SetGpuData();

	m_deviceContext->VSSetShader(_colorShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_colorShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_colorShader->GetInputLayout_Batch().Get());
	
	// 描画
	m_deviceContext->DrawIndexedInstanced(6, (UINT)_instBufVec.size(), 0, 0, 0);

	_instBufVec.clear();
}

void Direct3D::Draw2D_Batch(const Texture* texture)
{
	SetGpuData();

	m_deviceContext->VSSetShader(_texShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_texShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_texShader->GetInputLayout_Batch().Get());

	// テクスチャを、ピクセルシェーダーのスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, texture->m_shaderResourceview.GetAddressOf());

	// 描画
	m_deviceContext->DrawIndexedInstanced(6, (UINT)_instBufVec.size(), 0, 0, 0);

	_instBufVec.clear();
}

void Direct3D::DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView)
{
	SetGpuData();

	m_deviceContext->VSSetShader(_texShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_texShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_texShader->GetInputLayout_Batch().Get());

	// テクスチャを、ピクセルシェーダーのスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

	// 描画
	m_deviceContext->DrawIndexedInstanced(6, (UINT)_instBufVec.size(), 0, 0, 0);

	_instBufVec.clear();
}

void Direct3D::SetGpuData()
{
	// GPUにデータを転送
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	// サブリソースに含まれるデータへのポインターを取得し、そのサブリソースへの GPU アクセスを拒否します。
	// D3D11_MAP_WRITE_DISCARD...以前のバッファ内容を破棄して新しく書き込む
	if (SUCCEEDED(m_deviceContext->Map(_instanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		// vectorの内容をGPUのメモリへコピー
		memcpy(mappedSubresource.pData, _instBufVec.data(), sizeof(InstanceBuffer) * _instBufVec.size());
		m_deviceContext->Unmap(_instanceBuffer.Get(), 0);
	}
	else
	{
		Debug::Log("GPUへのデータ転送失敗[Direct3D::Draw2D_Batch()]");
		return;
	}

	// GPUパイプラインへのセット
	// スロット０：基本の形（四角形メッシュ）
	// スロット１：インスタンスデータ
	ID3D11Buffer* vBuffers[] = { _quadVertexBuffer.Get(), _instanceBuffer.Get() };
	UINT strides[] = { sizeof(VertexType2D), sizeof(InstanceBuffer) };
	UINT offsets[] = { 0, 0 };
	m_deviceContext->IASetVertexBuffers(0, 2, vBuffers, strides, offsets);


	// インデックスバッファのセット
	m_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

void Direct3D::AddMeshData(const std::vector<VertexType3D>& vertexVec)
{
	_vertexVec.push_back(vertexVec);
}

void Direct3D::Draw3D()
{
	if (_vertexVec.empty()) return;

	// --- 追加：3D描画用のステートを強制する ---
	D3D.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)GameSystem::WINDOW_WIDTH, (float)GameSystem::WINDOW_HEIGHT, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);

	// 2D描画の影響を排除（念のため）
	float blendFactor[4] = { 0, 0, 0, 0 };
	m_deviceContext->OMSetBlendState(nullptr, blendFactor, 0xffffffff);

	// シェーダーのセット
	m_deviceContext->VSSetShader(_meshShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_meshShader->GetPixelShader().Get(), 0, 0);

	// InputLayout
	m_deviceContext->IASetInputLayout(_meshShader->GetInputLayout3D().Get());

	// ConstantBuffer
	m_deviceContext->VSSetConstantBuffers(1, 1, _camBuf3D.GetAddressOf());


	// 頂点データ (時計回り)
	std::vector<VertexType3D> testVertices = {
		//   pos(x, y, z)          normal(x, y, z)      uv(u, v)
		{ {  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.5f, 0.0f } }, // 上
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } }, // 右下
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } }  // 左下
	};

	// インデックスデータ
	std::vector<unsigned int> testIndices = { 0, 1, 2 };


	// 頂点バッファをGPUに転送する
	D3D11_MAPPED_SUBRESOURCE mappedSubResource; // こいつにデータを載せて転送する

	// GPUのメモリをロックして、CPU側のポインタを取得
	if (SUCCEEDED(m_deviceContext.Get()->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource)))
	{
		// データをコピー（memcpyなど）
		//memcpy(mappedSubResource.pData, _meshData.vertexVec.data(), sizeof(VertexType3D) * _meshData.vertexVec.size());
		memcpy(mappedSubResource.pData, testVertices.data(), sizeof(VertexType3D) * testVertices.size());

		// ロックを解除（この瞬間にGPUへ転送されるイメージ）
		m_deviceContext->Unmap(_vertexBuffer.Get(), 0);
	}


	// モデル頂点バッファをGPUに転送する
	D3D11_MAPPED_SUBRESOURCE mappedSR;

	if (SUCCEEDED(m_deviceContext->Map(_modelBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSR)))
	{
		memcpy(mappedSR.pData, &_objectData, sizeof(ObjectData3D));
		m_deviceContext->Unmap(_modelBuffer.Get(), 0);
	}


	// インデックスバッファをGPUに転送する
	D3D11_MAPPED_SUBRESOURCE msr_indexBuf;
	if (SUCCEEDED(m_deviceContext->Map(_indexBuffer3D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr_indexBuf)))
	{
		//memcpy(msr_indexBuf.pData, _meshData._indexVec.data(), sizeof(unsigned int) * _meshData._indexVec.size());
		memcpy(msr_indexBuf.pData, testIndices.data(), sizeof(unsigned int) * testIndices.size());
		m_deviceContext->Unmap(_indexBuffer3D.Get(), 0);
	}


	// ふたつのバッファを頂点バッファとしてセット
	ID3D11Buffer* buffers[] = { _vertexBuffer.Get(), _modelBuffer.Get() };
	UINT strides[] = { sizeof(VertexType3D), sizeof(ObjectData3D) };
	UINT offsets[] = { 0, 0 };
	
	m_deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);


	// インデックスバッファをセット
	m_deviceContext->IASetIndexBuffer(_indexBuffer3D.Get(), DXGI_FORMAT_R32_UINT, 0);


	// 描画
	//m_deviceContext->DrawIndexedInstanced(_meshData._indexVec.size(), 1, 0, 0 , 0);
	m_deviceContext->DrawIndexedInstanced((UINT)testIndices.size(), 1, 0, 0 , 0);
}