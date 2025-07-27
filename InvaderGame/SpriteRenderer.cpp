#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer() : _color(DirectX::XMFLOAT4(1, 1, 1, 1)), m_texture { nullptr }
{

}

void SpriteRenderer::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void SpriteRenderer::SetColor(DirectX::XMFLOAT4 color)
{
	_color = color;
}

void SpriteRenderer::Update()
{
	D3D.SetColor(_color);

	Transform* transform = this->gameObject->GetTransform();
	//Vector2 draw_position = Vector2(transform->position.x - Camera::get_main()., transform->position.y);
	D3D.SetRect(transform->position.x, transform->position.y, transform->scale.x, transform->scale.y, transform->rotation);

	if (m_texture == nullptr)
	{
		D3D.Draw2D();
	}
	else
	{
		D3D.Draw2D(*m_texture);
	}
}