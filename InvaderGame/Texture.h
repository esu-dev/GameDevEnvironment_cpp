#pragma once

#include "DirectX.h"

class Texture
{
public:
	// シェーダーリソースビュー（画像データ読み取りハンドル）
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceview = nullptr;

	// 画像情報
	DirectX::TexMetadata m_texMetaData = {};

	bool Load(const std::string& fileName); // stdはC++標準の名前空間
};