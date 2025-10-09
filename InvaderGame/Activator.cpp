#include "Activator.h"

#include "GameEngine.h"

void Activator::Initialize()
{
	instanceMap["GameObject"] = []() -> Object* { return new GameObject(); };
}

void Activator::RegisterType(std::string typeString, std::function<Object*()> creationFunction)
{
	instanceMap[typeString] = creationFunction;
}

Object* Activator::CreateInstance(std::string typeString)
{
	auto creationFunc = instanceMap[typeString];
	if (creationFunc == nullptr)
	{
		Debug::Log("[%s]‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB", typeString);
		return nullptr;
	}
	return instanceMap[typeString]();
}