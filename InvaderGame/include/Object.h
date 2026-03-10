#pragma once

#define NAME(n) \
	const char* GetName() override \
	{ \
		return #n; \
	} \

#include "SerializedClass.h"

class GameObject;

class Object : public SerializedClass
{
public:
	static GameObject* Instantiate(GameObject* original);
	static void Destroy(GameObject* gameObject);

	Object();

	std::string instanceID = "initial instanceID";
	std::string name = "initial name"; // 削除するとエラーが出る。どこで参照されているのか不明。GameObjectの名前付けでしか利用していないから、ここに定義する必要はない。
	Object* original = nullptr;

	SERIALIZE3(SerializedClass,
		SERIALIZE_FIELD3(name, HIDE_INSPECTOR)
	)

	virtual const char* GetName() { return "not overrided"; }
	virtual void EditorUpdate() {}
};