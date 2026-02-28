#include "Canvas.h"

#include "GameEngine.h"

void Canvas::Update()
{
	this->get_transform()->position = Camera::GetMain()->get_transform()->position;
}