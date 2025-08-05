#include "Object.h"

#include "GameSystem.h"
#include "GameEngine.h"

void Object::Destroy(GameObject* gameObject)
{
	GAMESYS.AddDelayedExecution([=]() -> void
		{
			gameObject->scene->RemoveGameObject(gameObject);

			delete gameObject;
		});
}