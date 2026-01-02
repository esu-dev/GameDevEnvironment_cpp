#pragma once

#include "DirectX.h"
#include "Record.h"
#include "Color.h"
#include "Component.h"
#include "Texture.h"

class SpriteRenderer : public Component
{
public:
	NAME(SpriteRenderer)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_order),
		SERIALIZE_FIELD3(_color),
		SERIALIZE_FIELD3(m_texture),
		SERIALIZE_FIELD3(_isFlipX)
	)

	SpriteRenderer();
	void SetOrder(int order);
	void SetCanMove(bool canMove);
	void SetTexture(Texture* texture);
	void SetColor(DirectX::XMFLOAT4 color);
	void Update() override;
	void EditorUpdate() override;

	void SetFlip(bool flip);
	bool GetFlip();

private:
	int _order = 0;
	Color _color = Color(1, 1, 1, 1);
	Record<Texture*> m_texture;
	Record<bool> _isFlipX = false;

	bool _canMove = true;

	void Render(const Vector3& cameraPosition);
};

