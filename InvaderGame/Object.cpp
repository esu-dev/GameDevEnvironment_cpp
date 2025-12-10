#include "Object.h"

#include "GameSystem.h"
#include "GameEngine.h"
#include "Utility.h"

GameObject* Object::Instantiate(GameObject* original)
{
	if (original->IsPrefab)
	{
		SceneDataManager::LoadGameObjectClone(SceneManagement::SceneManager::GetActiveScene(), original->Serialize());
	}

	return nullptr;
}

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
