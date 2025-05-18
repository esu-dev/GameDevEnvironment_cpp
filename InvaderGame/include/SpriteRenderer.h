#pragma once

#include "framework.h"

#include "DirectX.h"
#include "Component.h"
#include "Texture.h"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	void SetTexture(Texture* texture);
	void SetColor(DirectX::XMFLOAT4 color);
	void Update() override;

private:
	DirectX::XMFLOAT4 _color;
	Texture* m_texture;
};

