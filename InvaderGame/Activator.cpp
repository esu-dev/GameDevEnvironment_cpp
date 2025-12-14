#include "Activator.h"

#include "GameEngine.h"
#include "VelocityController.h"
#include "SerializedSample.h"
#include "Player.h"

#define REGISTER_TYPE(type) \
	nameToInstanceMap[#type] = []() -> Object* { return new type(); }

void Activator::Initialize()
{
	REGISTER_TYPE(GameObject);
	REGISTER_TYPE(Camera);
	REGISTER_TYPE(Transform);
	REGISTER_TYPE(SpriteRenderer);
	REGISTER_TYPE(Animator);
	REGISTER_TYPE(Rigidbody2D);
	REGISTER_TYPE(BoxCollider2D);
	REGISTER_TYPE(AnimationClip);

	REGISTER_TYPE(VelocityController);
	REGISTER_TYPE(SerializedSample);
	REGISTER_TYPE(Player);
}

Object* Activator::CreateInstance(std::string typeString)
{
	auto creationFunc = nameToInstanceMap[typeString];
	if (creationFunc == nullptr)
	{
		Debug::Log("[Activator.cpp] %sÉNÉâÉXÇ™ìoò^Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒÅB", typeString.c_str());
		return nullptr;
	}
	return nameToInstanceMap[typeString]();
}

std::vector<std::string> Activator::GetObjectNameVec()
{
	std::vector<std::string> objectNameVec;
	for (auto& pair : nameToInstanceMap)
	{
		objectNameVec.push_back(pair.first);
	}

	return objectNameVec;
}

std::unordered_map<std::string, std::function<Object* ()>> Activator::nameToInstanceMap;