#pragma once

#include "framework.h"
#include "Object.h"

class Activator
{
public:
	static void Initialize();
	static Object* CreateInstance(std::string typeString);

private:
	static std::unordered_map<std::string, std::function<Object*()>> instanceMap;
};