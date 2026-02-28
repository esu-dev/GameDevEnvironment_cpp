#include "Object.h"

#include "GameSystem.h"
#include "GameEngine.h"
#include "Utility.h"

GameObject* Object::Instantiate(GameObject* original)
{
	std::vector<std::string> serializedData;
	AssetManager::SerializeGameObjectInChildren(serializedData, original);
	return SceneDataManager::LoadGameObjectsClone(SceneManagement::SceneManager::GetActiveScene(), serializedData);
}

void Object::Destroy(GameObject* gameObject)
{
	GAMESYS.AddDelayedExecution([=]() -> void
		{
			std::function<void(GameObject*)> deleteGameObject = [&](GameObject* target = nullptr) -> void
				{
					if (target == nullptr) return;

					for (Transform* child : target->GetTransform()->GetChildVector())
					{
						deleteGameObject(child->gameObject);
					}

					target->scene->RemoveGameObject(target);
					delete target;
				};

			deleteGameObject(gameObject);
		});
}

Object::Object()
{
	// ƒ‰ƒ“ƒ_ƒ€‚ÉinstanceID‚ðŒˆ’è‚·‚é
	instanceID = UuidGenerator::Generate();
}
