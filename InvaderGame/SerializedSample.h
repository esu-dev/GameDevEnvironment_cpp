#pragma once

#include "EngineFramework.h"
#include "EngineBehaviour.h"
#include "Vector3.h"

class SerializedSample : public EngineBehaviour
{
public:
	int _int;
	float _float;
	std::string _string;

	NAME(SerializedSample)
	SERIALIZE3(SerializedClass,
		SERIALIZE_FIELD3(_int),
		SERIALIZE_FIELD3(_string)
	)
};
