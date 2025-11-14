#pragma once

#include "framework.h"
#include "DirectX.h"
#include "Object.h"

class Texture : public Object
{
public:
	// シェーダーリソースビュー（画像データ読み取りハンドル）
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceview;

	// 画像情報
	DirectX::TexMetadata m_texMetaData = {};

	NAME(Texture)
	SERIALIZE3(Object,
	)

	Texture();
	Texture(std::string fileName);
	Texture(std::wstring path);
	Texture(std::string instanceID, std::wstring path);

	bool Load(const std::string& fileName); // stdはC++標準の名前空間
	bool Load(const std::wstring& path);
};