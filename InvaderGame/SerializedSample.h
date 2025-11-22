#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector3.h"

class SerializedSample : public Component
{
public:
	float hoge = 0;
	Vector3 fuga = Vector3();
	std::vector<int> intVector = {1, 2};

	NAME(SerializedSample)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(hoge),
		SERIALIZE_FIELD3(fuga),
		SERIALIZE_FIELD3(intVector)
	)
};
