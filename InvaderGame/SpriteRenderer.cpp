#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Camera.h"

SpriteRenderer::SpriteRenderer() : _color(DirectX::XMFLOAT4(1, 1, 1, 1)), m_texture { nullptr }
{

}

void SpriteRenderer::SetCanMove(bool canMove)
{
	_canMove = canMove;
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
	Transform* transform = this->gameObject->GetTransform();
	Vector3 draw_position = transform->position;
	if (_canMove)
	{
		draw_position = transform->position - Camera::get_main()->get_transform()->position;
	}

	// ƒJƒƒ‰‚Ì•`‰æ”ÍˆÍŠO‚È‚ç•`‰æ‚µ‚È‚¢
	Vector3 viewportPoint = Camera::WorldToViewportPoint(draw_position);
	Vector3 viewportScale = Camera::WorldToViewportPoint(this->gameObject->GetTransform()->scale);
	if (viewportPoint.x + viewportScale.x / 2 < -1 || viewportPoint.x - viewportScale.x / 2 > 1 ||
		viewportPoint.y + viewportScale.y / 2 < -1 || viewportPoint.y - viewportScale.y / 2 > 1)
	{
		//Debug::Log(L"•`‰æ‚È‚µ");
		return;
	}

	D3D.SetColor(_color);
	D3D.SetRect(draw_position.x, draw_position.y, transform->scale.x, transform->scale.y, transform->rotation);

	if (m_texture == nullptr)
	{
		D3D.Draw2D();
	}
	else
	{
		D3D.Draw2D(*m_texture);
	}
}