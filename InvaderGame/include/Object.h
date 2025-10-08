#pragma once

#include "framework.h"

class GameObject;

class Object
{
public:
	//static Object* Instantiate(Object* original);
	static void Destroy(GameObject* gameObject);

	Object();
	Object(std::string instanceID);

	std::string instanceID;
	std::string name;
};