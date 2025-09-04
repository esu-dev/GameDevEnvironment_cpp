#pragma once

#include "framework.h"

#include "DirectX.h"
#include "Component.h"
#include "Texture.h"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	void SetCanMove(bool canMove);
	void SetTexture(Texture* texture);
	void SetColor(DirectX::XMFLOAT4 color);
	void Update() override;

private:
	bool _canMove = true;
	DirectX::XMFLOAT4 _color;
	Texture* m_texture;
};

