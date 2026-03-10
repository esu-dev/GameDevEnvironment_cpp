#pragma once

#include "EngineFramework.h"
#include "Object.h"

class Activator
{
public:
	static Activator* Instance;
	static void Initialize();
	
	Activator();
	Object* CreateInstance(std::string typeString);
	std::vector<std::string> GetObjectNameVec();
	void AddNameToInstMap(std::string typeName, std::function<Object*()> instFunc);

private:
	std::unordered_map<std::string, std::function<Object*()>> nameToInstanceMap;
};