#pragma once

#include "framework.h"
#include "Component.h"
#include "Vector3.h"

class SerializedSample : public Component
{
public:
	class Foo : public SerializedClass
	{
	public:
		float a;

		SERIALIZE3(SerializedClass,
			SERIALIZE_FIELD3(a)
		)
	};

	float hoge = 0;
	Vector3 fuga = Vector3();
	std::vector<int> intVector = {};
	std::vector<Foo> fooVector = {};

	NAME(SerializedSample)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(hoge),
		SERIALIZE_FIELD3(fuga),
		SERIALIZE_FIELD3(intVector),
		SERIALIZE_FIELD3(fooVector)
	)
};
