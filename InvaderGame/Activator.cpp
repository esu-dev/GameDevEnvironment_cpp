#include "Activator.h"

#include "GameEngine.h"

#define REGISTER_TYPE(type) \
	instanceMap[#type] = []() -> Object* { return new type(); }

void Activator::Initialize()
{
	REGISTER_TYPE(GameObject);
	REGISTER_TYPE(Camera);
	REGISTER_TYPE(Transform);
	REGISTER_TYPE(SpriteRenderer);
}

Object* Activator::CreateInstance(std::string typeString)
{
	auto creationFunc = instanceMap[typeString];
	if (creationFunc == nullptr)
	{
		Debug::Log("[Activator.cpp] %sƒNƒ‰ƒX‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB", typeString.c_str());
		return nullptr;
	}
	return instanceMap[typeString]();
}

std::unordered_map<std::string, std::function<Object* ()>> Activator::instanceMap;