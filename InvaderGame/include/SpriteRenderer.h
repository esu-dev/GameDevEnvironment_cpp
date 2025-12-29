#pragma once

#include "DirectX.h"
#include "Component.h"
#include "Texture.h"

class SpriteRenderer : public Component
{
public:
	NAME(SpriteRenderer)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_order),
		SERIALIZE_FIELD3(m_texture)
	)

	SpriteRenderer();
	void SetCanMove(bool canMove);
	void SetTexture(Texture* texture);
	void SetColor(DirectX::XMFLOAT4 color);
	void Update() override;
	void EditorUpdate() override;

	void SetFlip(bool flip);
	bool GetFlip() const;
	void Render();

private:
	int _order = 0;

	bool _canMove = true;
	DirectX::XMFLOAT4 _color;
	Texture* m_texture;
	bool _isFlipX = false;
};

