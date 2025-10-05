#include "DemoMovement.h"

#include "GameEngine.h"

void DemoMovement::Update()
{
	if (Input::GetKey('A'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.Get().x - SPEED * Time::GetDelataTime(), this->get_transform()->position.Get().y);
	}
	else if (Input::GetKey('D'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.Get().x + SPEED * Time::GetDelataTime(), this->get_transform()->position.Get().y);
	}

	if (Input::GetKey('W'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.Get().x, this->get_transform()->position.Get().y + SPEED * Time::GetDelataTime());
	}
	else if (Input::GetKey('S'))
	{
		this->get_transform()->SetPosition(this->get_transform()->position.Get().x, this->get_transform()->position.Get().y - SPEED * Time::GetDelataTime());
	}
}