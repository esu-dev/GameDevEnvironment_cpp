#include "Object.h"

#include "GameSystem.h"
#include "GameEngine.h"
#include "Utility.h"

void Object::Destroy(GameObject* gameObject)
{
	GAMESYS.AddDelayedExecution([=]() -> void
		{
			gameObject->scene->RemoveGameObject(gameObject);

			delete gameObject;
		});
}

Object::Object()
{
	// ƒ‰ƒ“ƒ_ƒ€‚ÉinstanceID‚ðŒˆ’è‚·‚é
	instanceID = UuidGenerator::Generate();
}
Object::Object(std::string instanceID) { this->instanceID = instanceID; }