#include "framework.h"

#include <locale.h>

#include "Direct3D.h"

#include "Texture.h"

Texture::Texture() {}

Texture::Texture(std::string fileName)
{
	Load(fileName);
}

bool Texture::Load(const std::string& fileName)
{
	// マルチバイト文字列からワイド文字列へ変換
	setlocale(LC_CTYPE, "jpn"); // 地域設定を日本語に設定。日本語はマルチバイト文字であるから変換する必要がある。
	wchar_t wFileName[256]; // wchar_tはワイド文字（ユニコード）を表すためのデータ型
	size_t ret; // size_t はオブジェクトのバイト数を表現できる程度に十分に大きい符号なし整数型。
	mbstowcs_s(&ret, wFileName, fileName.c_str(), 256);

	// WIC(Windows Imaging Component)画像を読み込む
	auto image = std::make_unique<DirectX::ScratchImage>(); // インスタンス化
	if (FAILED(DirectX::LoadFromWICFile(wFileName, DirectX::WIC_FLAGS_NONE, &m_texMetaData, *image)))
	{
		// 失敗
		m_texMetaData = {};
		return false;
	}

	// ミップマップの生成
	if (m_texMetaData.mipLevels == 1) // 元のテクスチャにミップマップが存在しないならば
	{
		auto mipChain = std::make_unique<DirectX::ScratchImage>();
		if (SUCCEEDED(DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, *mipChain)))
		{
			image = std::move(mipChain);
		}
	}

	// リソースとシェーダーリソースビューを作成
	if (FAILED(DirectX::CreateShaderResourceView(D3D.m_device.Get(), image->GetImages(), image->GetImageCount(), m_texMetaData, &m_shaderResourceview)))
	{
		// 失敗
		m_texMetaData = {};
		return false;
	}

	// 成功
	return true;
}