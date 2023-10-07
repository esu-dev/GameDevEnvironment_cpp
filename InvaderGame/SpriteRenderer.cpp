#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{

}

void SpriteRenderer::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void SpriteRenderer::Update()
{
	D3D.Draw2D(*m_texture, 0, 0, 100.0 * 2 / 960, 100.0 * 2 / 540);
}