#pragma once

#include "SerializedClass.h"

class Color : public SerializedClass
{
public:
	float r, g, b, a;
	SERIALIZE3(SerializedClass,
		SERIALIZE_FIELD3(r),
		SERIALIZE_FIELD3(g),
		SERIALIZE_FIELD3(b),
		SERIALIZE_FIELD3(a)
	)

	Color(float r, float g, float b, float a);
};