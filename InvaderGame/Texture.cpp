#include "Texture.h"

Texture::Texture() {}

Texture::Texture(std::string fileName)
{
	Load(fileName);
}

Texture::Texture(std::wstring path)
{
	Load(path);
}

Texture::Texture(std::string instanceID, std::wstring path)
{
	this->instanceID = instanceID;

	Load(path);
}

bool Texture::Load(const std::string& fileName)
{
	// マルチバイト文字列からワイド文字列へ変換
	setlocale(LC_CTYPE, "jpn"); // 地域設定を日本語に設定。日本語はマルチバイト文字であるから変換する必要がある。
	wchar_t wFileName[256]; // wchar_tはワイド文字（ユニコード）を表すためのデータ型
	size_t ret; // size_t はオブジェクトのバイト数を表現できる程度に十分に大きい符号なし整数型。
	mbstowcs_s(&ret, wFileName, fileName.c_str(), 256);

 	return Load(wFileName);
}

bool Texture::Load(const std::wstring& path)
{
	// WIC(Windows Imaging Component)画像を読み込む
	auto image = std::make_unique<DirectX::ScratchImage>(); // インスタンス化
	// WIC_FLAGS_NONE...ガンマ補正などをしない設定
	// WIC_FLAGS_FORCE_SRGB
	// WIC_FLAGS_IGNORE_SRGB...カラーマネジメント情報（sRGBチャンクやガンマ値）を無視して、ただの数値データとして読み込む
	if (FAILED(DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_IGNORE_SRGB, &m_texMetaData, *image)))
	{
		// 失敗
		m_texMetaData = {};
		Debug::Log("WICの読み込みに失敗しました。");
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
		else
		{
			Debug::Log("みっぷマップの生成に失敗しました。");
		}
	}

	// リソースとシェーダーリソースビューを作成
	if (FAILED(DirectX::CreateShaderResourceView(Direct3D::GetInstance()._device.Get(), image->GetImages(), image->GetImageCount(), m_texMetaData, &m_shaderResourceview)))
	{
		// 失敗
		m_texMetaData = {};
		Debug::Log("ShaderResourceViewの作成に失敗しました。");
		return false;
	}

	// 成功
	return true;
}