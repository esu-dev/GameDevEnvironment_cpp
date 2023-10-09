// 参考　https://gamesgard.com/directx11_lesson02/

// 一度しかインクルードされないようにコンパイラに指定する
#pragma once

#include "DirectX.h"

#include "Vector3.h"
#include "Quaternion.h"

class Texture;
class Quaternion;

// 2D用頂点構造体
struct VertexType2D
{
	DirectX::XMFLOAT3 Pos; // 座標
	DirectX::XMFLOAT2 UV; // UV座標
};

struct ConstantBuffer
{
	DirectX::XMMATRIX m_WP;
};

//=========================================
// Direct3Dクラス
//=========================================
class Direct3D
{
public:

	// ※変数は今回は全てpublicにしますが、本来はprivateで隠すべき※

	// Direct3Dデバイス
	ComPtr<ID3D11Device> m_device;

	// Direct3Dデバイスコンテキスト
	// ディスプレイの描画属性に関する情報を保持する Windows のデータ構造体
	ComPtr<ID3D11DeviceContext>	m_deviceContext;

	// スワップチェイン
	// ひとつにデバイスから複数のビューを表現できる
	ComPtr<IDXGISwapChain> m_swapChain;

	// バックバッファーのRTビュー
	// RenderTargetView:レンダリングパイプラインからアウトプットされるピクセル情報を保存するViewです。
	// レンダリングパイプラインとは、モデルデータの入力から出力までのレンダリングのための加工手順のこと
	ComPtr<ID3D11RenderTargetView> m_backBufferView;

	//--------------------------------------------
	// Direct3Dを初期化し、使用できるようにする関数
	// hWnd		: ウィンドウハンドル
	// width	: 画面の幅
	// height	: 画面の高さ
	//--------------------------------------------
	bool Initialize(HWND hWnd, int width, int height);

	// 2D描画用のシェーダー
	ComPtr<ID3D11VertexShader> m_spriteVS = nullptr;
	ComPtr<ID3D11PixelShader> m_spritePS = nullptr;
	ComPtr<ID3D11InputLayout> m_spriteInputLayout = nullptr;

	ComPtr <ID3D11Buffer> m_vbSquare; // 四角形用頂点バッファ
	ComPtr<ID3D11Buffer> constantBuffer;

	// 2D描画モードにする
	void ChangeMode_2D();

	void SetRect(float x, float y, float w, float h);
	void SetRect(float x, float y, float w, float h, Quaternion quaternion);

	// 2D描画
	void Draw2D(const Texture& texture);

	void DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView);

	//=========================================
	// 今回このクラスは、どこからでもアクセスできるように
	// シングルトンパターンにします
	// ↓↓↓以下、シングルトンパターンのコード
	//=========================================
private:
	// 唯一のインスタンス用のポインタ
	// 処理の高速化で使用される手法のひとつ
	// 呼び出し時に関数先に行かずに関数側が呼び出し側に展開される
	static inline Direct3D* s_instance;

	// コンストラクタはprivateにする
	Direct3D() {}
public:
	// インスタンス作成
	static void CreateInstance()
	{
		DeleteInstance();

		s_instance = new Direct3D();
	}

	// インスタンス削除
	static void DeleteInstance()
	{
		// nullptr:ヌルポインタ値
		if (s_instance != nullptr)
		{
			// delete演算子
			// new演算子で生成されたオブジェクトにブロックを開放する
			delete s_instance;
			s_instance = nullptr;
		}
	}

	// 唯一のインスタンスを取得
	// &はポインタ
	static Direct3D& GetInstance()
	{
		return *s_instance;
	}
};

// Direct3Dの唯一のインスタンスを簡単に取得するためのマクロ
#define D3D Direct3D::GetInstance()