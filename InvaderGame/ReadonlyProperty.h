#pragma once

#include "EngineFramework.h"
#include "PropertyBase.h"

class Vector2;

template <typename T>
class ReadonlyProperty : public PropertyBase<T>
{
public:
	ReadonlyProperty(std::function<T()> get) : PropertyBase(get) { }
};

