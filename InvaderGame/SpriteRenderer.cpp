#include "SpriteRenderer.h"

#include "EditorCamera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Camera.h"
#include "GameSystem.h"

SpriteRenderer::SpriteRenderer() : m_texture { nullptr }
{
}

void SpriteRenderer::SetOrder(int order)
{
	_order = order;
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
	_color = Color(color.x, color.y, color.z, color.w);
}

void SpriteRenderer::Update()
{
	// Update ‚ÍˆÊ’u‚âó‘Ô‚ÌXV‚¾‚¯‚É‚µ‚ÄA•`‰æ“o˜^‚ðs‚¤
	// “o˜^ŠÖ”‚ðì¬‚µ‚Ä GameSystem ‚É‡˜•t‚«‚Å“n‚·
	GameSystem::GetInstance().AddRenderingData(_order, [this]() -> void { this->Render2(Camera::get_main()->GetTransform()->position); });
}

void SpriteRenderer::EditorUpdate()
{
	GameSystem::GetInstance().AddRenderingData(_order, [this]() -> void { this->Render2(EditorCamera::GetPosition()); });
}

void SpriteRenderer::Render(const Vector3& cameraPosition)
{
	Transform* transform = this->gameObject->GetTransform();
	Vector3 draw_position = transform->position;
	if (_canMove)
	{
		draw_position = transform->position - cameraPosition;
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

	D3D.SetColor({_color.r, _color.g, _color.b, _color.a});
	D3D.SetRect(draw_position.x, draw_position.y, transform->scale.x, transform->scale.y, transform->rotation);

	if (m_texture == nullptr)
	{
		D3D.Draw2D();
	}
	else
	{
		if (_isFlipX)
		{
			D3D.Draw2D_Flip(m_texture);
		}
		else
		{
			D3D.Draw2D(m_texture);
		}
	}
}

void SpriteRenderer::Render2(const Vector3& cameraPosition)
{
	Transform* transform = this->gameObject->GetTransform();
	Vector3 draw_position = transform->position;
	if (_canMove)
	{
		draw_position = transform->position - cameraPosition;
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

	Direct3D::GetInstance().SetInstanceData(
		{ draw_position.x, draw_position.y },
		{ transform->scale.x, transform->scale.y },
		transform->rotation,
		{ _color.r, _color.g, _color.b, _color.a },
		_isFlipX
	);

	if (m_texture == nullptr)
	{
		Direct3D::GetInstance().Draw2D_Batch();
	}
	else
	{
		Direct3D::GetInstance().Draw2D_Batch(m_texture);
	}
}

void SpriteRenderer::SetFlip(bool flip)
{
	_isFlipX = flip;
}

bool SpriteRenderer::GetFlip()
{
	return _isFlipX.Get();
}