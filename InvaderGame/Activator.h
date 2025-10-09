#pragma once

#include "framework.h"
#include "Object.h"

#define REGISTER_TYPE(type) \
	Activator::RegisterType(#type, []() -> Object* { return new type(); })

class Activator
{
public:
	static void Initialize();
	static void RegisterType(std::string typeString, std::function<Object*()> creationFunction);
	static Object* CreateInstance(std::string typeString);

private:
	static std::unordered_map<std::string, std::function<Object*()>> instanceMap;
};