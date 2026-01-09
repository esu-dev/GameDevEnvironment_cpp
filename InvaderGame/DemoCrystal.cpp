#include "DemoCrystal.h"
#include "GameObject.h"

void DemoCrystal::OnTriggerStay2D(GameObject* other)
{
	this->gameObject->SetActive(false);
}