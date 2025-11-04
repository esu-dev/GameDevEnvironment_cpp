#pragma once

#include "Component.h"
#include "Vector3.h"

class SerializedSample : public Component
{
public:
	float hoge = 0;
	Vector3 fuga = Vector3();

	NAME(SerializedSample)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(hoge),
		SERIALIZE_FIELD3(fuga)
	)
};
