#include "VelocityController.h"

#include "GameEngine.h"

void VelocityController::Start()
{
	Rigidbody2D* rigidbody = this->GetComponent<Rigidbody2D>();
	rigidbody->velocity = _initialVelocity;
}