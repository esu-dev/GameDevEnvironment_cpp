#include "DemoMovement.h"

#include "GameEngine.h"

void DemoMovement::Update()
{
	if (Input::GetKey('A'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.x - SPEED * Time::GetDelataTime(), this->get_transform()->position.y);
	}
	else if (Input::GetKey('D'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.x + SPEED * Time::GetDelataTime(), this->get_transform()->position.y);
	}

	if (Input::GetKey('W'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y + SPEED * Time::GetDelataTime());
	}
	else if (Input::GetKey('S'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.x, this->get_transform()->position.y - SPEED * Time::GetDelataTime());
	}
}