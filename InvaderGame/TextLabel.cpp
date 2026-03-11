#include "TextLabel.h"

#include "GameEngine.h"
#include "EditorCamera.h"
#include "GameSystem.h"
#include <climits>
#include <map>

std::map<TextLabel::CacheKey, ComPtr<ID3D11ShaderResourceView>> TextLabel::_srvCache;

TextLabel::TextLabel()
{

}

void TextLabel::SetCanMove(bool canMove)
{
	_canMove = canMove;
}

void TextLabel::SetText(std::string text)
{
	_text.swap(text);
}

void TextLabel::SetFontSize(int fontSize)
{
	FontSize = fontSize;
}

void TextLabel::SetTextAlign(TextAlign textAlign)
{
	_textAlign = textAlign;
}

void TextLabel::Update()
{
	GameSystem::GetInstance().AddRenderingData(INT_MAX, [this]() -> void { this->Render(Camera::GetMain()->GetTransform()->position); });
}

void TextLabel::EditorUpdate()
{
	GameSystem::GetInstance().AddRenderingData(INT_MAX, [this]() -> void { this->Render(EditorCamera::GetPosition()); });
}

void TextLabel::MakeShaderResourceViewOf(wchar_t c, ComPtr<ID3D11ShaderResourceView> *srv)
{
	// -----------------------
	// フォントハンドルの生成
	// -----------------------
	// フォントハンドル...フォントを操作するためのもの？
	int fontWeight = FW_NORMAL;

	// LOGFONT.. フォントの属性を定義する構造体
	LOGFONT logFont = {
		-FontSize * _resolution, 0, 0, 0, // height, width, Escapement, Orientation
		fontWeight, 0, 0, 0, // Weight(0~1000)...太字, Italic, Underline, StrikeOut（取り消し線）
		ANSI_CHARSET, // CharSet...文字コードみたいな
		OUT_TT_ONLY_PRECIS, // OutPrecision...出力精度
		CLIP_DEFAULT_PRECIS, // ClipPrecision...クリッピングの有効桁数
		CLEARTYPE_QUALITY, // Quality...出力品質
		DEFAULT_PITCH | FF_MODERN, // PitchAndFamily...フォントのピッチとファミリ
		TEXT("Georgia") //TEXT("ⅯＳ　Ｐ明朝") // FaceName...フォントの書体名を指定する文字列
	};

	// フォントハンドルを生成
	// 論理フォントを作成する
	// 論理フォントは実際にインストールされている物理フォントに対応するフォント
	HFONT hFont = CreateFontIndirectW(&logFont);

	// 現在のウィンドウに適用させる
	// デバイスにフォントを持たせないとGetGlyphOutlineW関数はエラーとなる
	// HDC...デバイスコンテキストのハンドル
	// GetDC()...HDCを取得する関数
	HDC hdc = GetDC(NULL);

	// SelectObject()...指定されたデバイスコンテキストのオブジェクトを選択する。
	// 新しいオブジェクトは以前の同じ型のオブジェクトと入れ替えられる。
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	//const wchar_t* c = TEXT("あ");
	UINT code = static_cast<UINT>(c);

	TEXTMETRIC textmetric;
	GetTextMetrics(hdc, &textmetric);
	GLYPHMETRICS glyphmetrics; // 作成されたフォントビットマップに関する情報を格納する構造体

	// CONST...constの定義（なぜに？）
	// MAT2...文字を回転させる際の変換行列を表す配列
	CONST MAT2 mat = { {0, 1}, {0, 0}, {0, 0}, {0, 1} };

	// ここでGetGlyphOutline関数からビットマップを取得
	// DWORD...unsigned longの定義
	// GetGlyphOutlineW関数の戻り値はバッファーサイズ
	DWORD size = GetGlyphOutlineW(hdc, code, GGO_GRAY4_BITMAP, &glyphmetrics, 0, NULL, &mat);


	// BYTE...unnsigned charの定義
	// BMP...BitMaP
	BYTE* pFontBMP = new BYTE[size];
	
	GetGlyphOutlineW(hdc, code, GGO_GRAY4_BITMAP, &glyphmetrics, size, pFontBMP, &mat);

	// デバイスコンテキストとフォントハンドルを解放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	// テクスチャ作成
	int fontWidth = glyphmetrics.gmCellIncX;
	int fontHeight = textmetric.tmHeight;
	int mipLevels = 1;

	D3D11_TEXTURE2D_DESC desc; // 2Dテクスチャに関する設定をする構造体
	desc.Width = fontWidth;
	desc.Height = fontHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texture = NULL;

	if (FAILED(Direct3D::GetInstance()._device->CreateTexture2D(&desc, NULL, texture.GetAddressOf())))
	{
		// OKボタンと×ボタンのあるメッセージボックスを表示させる。
		MessageBox(NULL, L"フォント用テクスチャを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}

	// フォント情報をテクスチャに書き込む
	D3D11_MAPPED_SUBRESOURCE mappedSubresource; // サブリソースデータへのアクセスを提供する
	// Map関数...サブリソースに含まれるデータへのポインターを取得し、そのサブリソースへのGPUアクセスを拒否する。
	// Get関数...ComPtrの中身をポインタ型で返す
	Direct3D::GetInstance().m_deviceContext->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	BYTE* pBits = (BYTE*)mappedSubresource.pData;
	int offset_x = glyphmetrics.gmptGlyphOrigin.x;
	int offset_y = textmetric.tmAscent - glyphmetrics.gmptGlyphOrigin.y;
	int bitMapWidth = glyphmetrics.gmBlackBoxX + (4 - (glyphmetrics.gmBlackBoxX % 4)) % 4;
	int bitMapHeight = glyphmetrics.gmBlackBoxY;
	int Level = 17; // 17段階の色で構成する
	int x, y;
	DWORD Alpha, Color;
	// pBitsの初期化
	// memset関数...指定文字列の最初から指定文字数だけ指定した値にする
	memset(pBits, 0, mappedSubresource.RowPitch * textmetric.tmHeight);
	for (y = offset_y; y < offset_y + bitMapHeight; y++)
	{
		for (x = offset_x; x < offset_x + bitMapWidth; x++)
		{
			Alpha = (255 * pFontBMP[x - offset_x + bitMapWidth * (y - offset_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);

			memcpy((BYTE*)pBits + mappedSubresource.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
		}
	}
	// リソースへのポインターを無効にし、そのリソースへのGPUアクセスをサイド有効にする
	Direct3D::GetInstance().m_deviceContext->Unmap(texture.Get(), 0);

	// メモリ開放
	delete[] pFontBMP;
	pFontBMP = nullptr;

	//シェーダーリソースビューを作成する
	//シェーダーリソースビュー...テクスチャのリソース内のデータにアクセスするもの。
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc; // シェーダーリソースビューについて設定する構造体
	shaderResourceViewDesc.Format = desc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = desc.MipLevels; // テクスチャのミップマップレベルの最大数
	// ミップマップ...LODで処理を高速化する手法のこと

	if (FAILED(Direct3D::GetInstance()._device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, srv->GetAddressOf())))
	{
		MessageBox(NULL, L"シェーダーリソースビューを作成できませんでした。", L"エラーウィンドウ", MB_OK | MB_ICONERROR);
		return;
	}
}

void TextLabel::Render(const Vector3& cameraPosition)
{
	Quaternion rotation = gameObject->GetTransform()->rotation;

	int i = 0;
	float width = FontSize / Camera::Magnification * 0.8f;

	Vector3 textStartPos = gameObject->GetTransform()->position;
	if (_textAlign == TextAlign::Center)
	{
		textStartPos = gameObject->GetTransform()->position.Get().AddX(-((_text.length() - 1) * width / 2));
	}
	else if (_textAlign == TextAlign::Right)
	{
		textStartPos = gameObject->GetTransform()->position.Get().AddX(-((_text.length() - 1) * width));
	}

	for (wchar_t c : _text)
	{
		CacheKey key = { c, FontSize };
		if (_srvCache.find(key) == _srvCache.end())
		{
			ComPtr<ID3D11ShaderResourceView> srv;
			MakeShaderResourceViewOf(c, &srv);
			_srvCache[key] = srv;
		}

		ComPtr<ID3D11ShaderResourceView> srv = _srvCache[key];
		
		Vector3 drawPosition = rotation.Mult(Vector3(textStartPos.x + i * width, textStartPos.y, 0));
		if (_canMove)
		{
			drawPosition = drawPosition - cameraPosition;
		}

		Direct3D::GetInstance().SetInstanceData(
			{ drawPosition.x, drawPosition.y },
			{ width, FontSize / Camera::Magnification },
			rotation,
			{ 1, 1, 1, 1 },
			false
		);

		Direct3D::GetInstance().DrawChar(srv);

		i++;
	}
}