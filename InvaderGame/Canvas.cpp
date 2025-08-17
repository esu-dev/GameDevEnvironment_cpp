#include "Canvas.h"

#include "GameEngine.h"

void Canvas::Update()
{
	this->get_transform()->position = Camera::main->get_transform()->position;
}