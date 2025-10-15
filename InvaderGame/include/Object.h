#pragma once

#define NAME(n) \
	std::string GetName() override \
	{ \
		return #n; \
	}

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

	std::string instanceID = "initial instanceID";
	std::string name = "initial name"; // 削除するとエラーが出る。どこで参照されているのか不明

	virtual std::string GetName() { return "not overrided"; };


protected:
	std::unordered_map<std::string, std::function<void(std::string)>> variableMap;
};