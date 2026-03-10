#pragma once

#include "EString.h"
#include "Debug.h"
#include "EngineBehaviour.h"

class EStringTest : public EngineBehaviour
{
public:
	NAME(EStringTest)

	void Start() override
	{
		EString string = "aiueo";
		Debug::Log("EStringTest: output = %s", string.c_str());
	};
};