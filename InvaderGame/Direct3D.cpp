// 参考　https://gamesgard.com/directx11_lesson02/

#include "framework.h"

#include "Direct3D.h"
#include "Shader.h"

#include "GameEngine.h"
#include "GameSystem.h"


Direct3D::Direct3D() :
	_textureShader(new Shader(L"Shader/SpriteShader.hlsl", "VS", "PS")),
	_colorShader(new Shader(L"Shader/SpriteShader.hlsl", "VS", "PS_Color")),
	_textureShaderFlip(new Shader(L"Shader/SpriteShader.hlsl", "VS_Flip", "PS")),
	_texShader_Batch(new Shader(L"Shader/SpriteShader.hlsl", "VS_New", "PS_New")),
	_colorShader_Batch(new Shader(L"Shader/SpriteShader.hlsl", "VS_New", "PS_New_Color")) {}

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
	//=====================================================
	// ファクトリー作成(ビデオ グラフィックの設定の列挙や指定に使用されるオブジェクト)
	//=====================================================
	// ComPtrはGenericsということか？
	ComPtr<IDXGIFactory> factory;

	// Createしようとして効果が無かったら（既にある）何もしない。
	// COMインターフェース関連の関数の戻り値ではHRESULT型が利用されることが多い。
	// FAILEDマクロはHRESULT型の戻り値を判定することができる。
	// メソッドは成功したが、効果が無かったときにs_FALSEが返される。
	// これを受け取ると、FAILEDマクロはtrueを返す。
	// HRESULT型はlong型の数値
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
	// DEBUGビルド時はDirect3Dのデバッグを有効にする
	// (すごく重いが細かいエラーがわかる)
	// D3D11_CREATE_DEVICE_DEBUG:enumの中身のひとつ。
	// デバッグレイヤーをサポートするデバイスを作成するフラグを立てられる。
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// FeatureLevelを配列で保持しておけるもの
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
			&m_device, // ID3D11Device**
			&futureLevel, // D3D_FEATURE_LEVEL*
			&m_deviceContext // ID3D11DeviceContext**
		)))
	{
		return false;
	}

	//=====================================================
	// スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
	//=====================================================
	// バックバッファとはディスプレイに表示されない仮想的な画面のこと
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
	// ComPtrがアロー演算子をオーバーロードしているから
	if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
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
	if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
	{
		return false;
	}

	//=====================================================
	// デバイスコンテキストに描画に関する設定を行っておく
	//=====================================================

	// バックバッファをRTとしてセット
	// ここでGetAddressOf()を使うのは、ComPtrでは&がオーバーロードされているから
	m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

	// ビューポートの設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);

	//=====================================================
	// シェーダーの作成
	//=====================================================
	_textureShader->CreateShader(*m_device.Get());
	_colorShader->CreateShader(*m_device.Get());
	_textureShaderFlip->CreateShader(*m_device.Get());
	_texShader_Batch->CreateShader(*m_device.Get());
	_colorShader_Batch->CreateShader(*m_device.Get());

	return true;
}

void Direct3D::ChangeMode_2D()
{
	// 四角形用頂点バッファを作成
	if (m_vbSquare == nullptr)
	{
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
		vbDesc.ByteWidth = sizeof(VertexType2D) * 4;	// 作成するバッファのバイトサイズ
		vbDesc.MiscFlags = 0;							// その他のフラグ
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;				// 作成するバッファの使用法
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_device->CreateBuffer(&vbDesc, nullptr, &m_vbSquare);
	}

	// 頂点バッファを描画で使えるようにセットする
	UINT stride = sizeof(VertexType2D); // １頂点のバイトサイズを教える
	UINT offset = 0;
	D3D.m_deviceContext->IASetVertexBuffers(0, 1, m_vbSquare.GetAddressOf(), &stride, &offset); // デバイスコンテキストに頂点バッファをセット


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
	m_device->CreateBuffer(&vDesc, &vData, _quadVertexBuffer.GetAddressOf());


	// プロミティブ・トポロジーをセット
	// ４頂点で四角形を描画できる設定
	D3D.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// 定数バッファの作成
	D3D11_BUFFER_DESC bufferDesk = {};
	bufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesk.ByteWidth = sizeof(CameraBuffer);
	bufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(m_device->CreateBuffer(&bufferDesk, nullptr, _cameraBuffer.GetAddressOf())))
	{
		MessageBox(NULL, L"定数バッファを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// 定数バッファへの書き込み
	// よく考えたら将来的には、毎フレーム更新が必須だわ
	CameraBuffer constantBuffer;
	constantBuffer.projMat = DirectX::XMMatrixOrthographicLH(GameSystem::WINDOW_WIDTH / Camera::Magnification, GameSystem::WINDOW_HEIGHT / Camera::Magnification, 0.0f, 1.0f);
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	if (SUCCEEDED(m_deviceContext->Map(_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		memcpy(mappedSubresource.pData, &constantBuffer, sizeof(CameraBuffer));
		m_deviceContext->Unmap(_cameraBuffer.Get(), 0);
	}
	else
	{
		MessageBox(NULL, L"定数バッファを設定できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// 頂点シェーダーに定数バッファを設定
	m_deviceContext->VSSetConstantBuffers(0, 1, _cameraBuffer.GetAddressOf());


	// カラー定数バッファの作成
	// 定数情報の追加
	D3D11_BUFFER_DESC colorBufferDesk = {};
	ZeroMemory(&colorBufferDesk, sizeof(D3D11_BUFFER_DESC));
	colorBufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesk.ByteWidth = sizeof(CameraBuffer);
	colorBufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesk.MiscFlags = 0;
	colorBufferDesk.StructureByteStride = 0;

	// 定数バッファの設定
	if (FAILED(m_device->CreateBuffer(&colorBufferDesk, nullptr, _colorBuffer.GetAddressOf())))
	{
		MessageBox(NULL, L"カラー定数バッファを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}


	// インスタンスバッファの作成
	D3D11_BUFFER_DESC instanceBufferDesk = {};
	instanceBufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesk.ByteWidth = sizeof(InstanceBuffer) * 1000; // 1000個の配列にできるということか？
	instanceBufferDesk.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesk.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	m_device->CreateBuffer(&instanceBufferDesk, nullptr, _instanceBuffer.GetAddressOf());


	// 初期値設定
	SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));


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
		m_device->CreateSamplerState(&desc, &state);

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
	if (FAILED(m_device->CreateBlendState(&blendDesc, blendState.GetAddressOf())))
	{
		MessageBox(NULL, L"ブレンドステートを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// ブレンドをセット
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	m_deviceContext->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
}

void Direct3D::SetRect(float x, float y, float w, float h)
{
	float hW = w * 0.5f;
	float hH = h * 0.5f;

	// 頂点データ作成
	VertexType2D v[4] = {
		{{x - hW, y - hH, 0}, {0, 1}},	// 左下
		{{x - hW, y + hH, 0}, {0, 0}},	// 左上
		{{x + hW, y - hH, 0}, {1, 1}},	// 右下
		{{x + hW, y + hH, 0}, {1, 0}},	// 右上
	};

	// 頂点バッファにデータを書き込む
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_deviceContext->Map(m_vbSquare.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// データコピー
		memcpy_s(pData.pData, sizeof(v), &v[0], sizeof(v));

		m_deviceContext->Unmap(m_vbSquare.Get(), 0);
	}
}

void Direct3D::SetRect(float x, float y, float w, float h, Quaternion quaternion)
{
	float hW = w * 0.5f;
	float hH = h * 0.5f;

	Vector3 position = Camera::WorldToViewportPoint(Vector3(x, y, 0));

	Vector3 direction_leftDown = Vector3(-hW, -hH, 0);
	Vector3 direction_leftUp = Vector3(-hW, hH, 0);
	Vector3 direction_rightDown = Vector3(hW, -hH, 0);
	Vector3 direction_rightUp = Vector3(hW, hH, 0);

	Vector3 leftDown = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_leftDown));
	Vector3 leftUp = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_leftUp));
	Vector3 rightDown = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_rightDown));
	Vector3 rightUp = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_rightUp));

	// 頂点データ作成
	VertexType2D v[4] = {
		{{leftDown.x, leftDown.y, 0}, {0, 1}},	// 左下
		{{leftUp.x, leftUp.y, 0}, {0, 0}},	// 左上
		{{rightDown.x, rightDown.y, 0}, {1, 1}},	// 右下
		{{rightUp.x, rightUp.y, 0}, {1, 0}},	// 右上
	};

	// 頂点バッファにデータを書き込む
	D3D11_MAPPED_SUBRESOURCE pData;
	// Map...サブリソースに含まれるデータへのポインターを取得し、そのサブリソースへの GPU アクセスを拒否します。
	if (SUCCEEDED(m_deviceContext->Map(m_vbSquare.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// 頂点データをサブリソースにコピー
		memcpy_s(pData.pData, sizeof(v), &v[0], sizeof(v));

		m_deviceContext->Unmap(m_vbSquare.Get(), 0);
	}
}

void Direct3D::SetColor(DirectX::XMFLOAT4 color)
{
	// 更新するデータを用意
	ColorBuffer colorBuffer;
	colorBuffer.color = color;

	// 定数バッファにデータを書き込む
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	if (SUCCEEDED(m_deviceContext->Map(_colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		memcpy(mappedSubresource.pData, &colorBuffer, sizeof(ColorBuffer));
		m_deviceContext->Unmap(_colorBuffer.Get(), 0);
	}
	else
	{
		MessageBox(NULL, L"カラー設定時に、定数バッファを更新できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// ピクセルシェーダーに定数バッファを設定
	m_deviceContext->PSSetConstantBuffers(1, 1, _colorBuffer.GetAddressOf());
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


void Direct3D::Draw2D()
{
	m_deviceContext->VSSetShader(_colorShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_colorShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_colorShader->GetInputLayout().Get());

	m_deviceContext->Draw(4, 0);
}

void Direct3D::Draw2D(const Texture* texture)
{
	m_deviceContext->VSSetShader(_textureShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_textureShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_textureShader->GetInputLayout().Get());

	// テクスチャを、ピクセルシェーダーのスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, texture->m_shaderResourceview.GetAddressOf());

	// デバイスコンテキストくん、上記のセットした内容で描画してください、とお願いする
	m_deviceContext->Draw(4, 0); // 頂点の数
}

/// <summary>
/// フリップ用シェーダーを使用して2Dテクスチャを描画します
/// </summary>
/// <param name="texture">描画するテクスチャへのポインタ</param>
void Direct3D::Draw2D_Flip(const Texture* texture)
{
    m_deviceContext->VSSetShader(_textureShaderFlip->GetVertexShader().Get(), 0, 0);
    m_deviceContext->PSSetShader(_textureShaderFlip->GetPixelShader().Get(), 0, 0);
    m_deviceContext->IASetInputLayout(_textureShaderFlip->GetInputLayout().Get());

    // テクスチャを、ピクセルシェーダーのスロット0にセット
    m_deviceContext->PSSetShaderResources(0, 1, texture->m_shaderResourceview.GetAddressOf());

	// 描画
    m_deviceContext->Draw(4, 0);
}

void Direct3D::Draw2D_Batch()
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


	m_deviceContext->VSSetShader(_colorShader_Batch->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_colorShader_Batch->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_colorShader_Batch->GetInputLayout_Batch().Get());

	
	// 描画
	m_deviceContext->DrawInstanced(4, _instBufVec.size(), 0, 0);


	_instBufVec.clear();
}

void Direct3D::Draw2D_Batch(const Texture* texture)
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


	m_deviceContext->VSSetShader(_texShader_Batch->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_texShader_Batch->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_texShader_Batch->GetInputLayout_Batch().Get());

	// テクスチャを、ピクセルシェーダーのスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, texture->m_shaderResourceview.GetAddressOf());


	// 描画
	m_deviceContext->DrawInstanced(4, _instBufVec.size(), 0, 0);


	_instBufVec.clear();
}

void Direct3D::DrawRect(const Vector2& center, const Vector2& size, const Quaternion& rotation, DirectX::XMFLOAT4 color)
{
	SetColor(color);
	SetRect(center.x, center.y, size.x, size.y, rotation);
	Draw2D();
}

void Direct3D::DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView)
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


	m_deviceContext->VSSetShader(_texShader_Batch->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_texShader_Batch->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_texShader_Batch->GetInputLayout_Batch().Get());

	// テクスチャを、ピクセルシェーダーのスロット0にセット
	m_deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());


	// 描画
	m_deviceContext->DrawInstanced(4, _instBufVec.size(), 0, 0);


	_instBufVec.clear();
}