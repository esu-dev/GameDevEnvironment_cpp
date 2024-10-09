#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"

SpriteRenderer::SpriteRenderer()
	: m_texture{nullptr}
{

}

void SpriteRenderer::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void SpriteRenderer::Update()
{
	this->gameObject->GetTransform()->Update();

	if (m_texture == nullptr)
	{
		D3D.Draw2D();
	}
	else
	{
		D3D.Draw2D(*m_texture);
	}
}