#pragma once

#include "framework.h"
#include "SerializedClass.h"

class GameObject;

class Object : public SerializedClass
{
public:
	//static Object* Instantiate(Object* original);
	static void Destroy(GameObject* gameObject);

	Object();
	Object(std::string instanceID);

	std::string instanceID;
	std::string name;


protected:
	std::unordered_map<std::string, std::function<void(std::string)>> variableMap;
};