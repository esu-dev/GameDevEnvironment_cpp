#pragma once

#include "DirectX.h"
#include "Component.h"
#include "Texture.h"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	void SetTexture(Texture* texture);
	void Update() override;

private:
	Texture* m_texture;
};

