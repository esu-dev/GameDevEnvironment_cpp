#pragma once

#include <stdlib.h>
#include <functional>

#include "PropertyBase.h"

class Vector2;

template <typename T>
class Property : public PropertyBase
{
public:
	using value_type = T;

	Property() {}
	Property(std::function<T()> getter) { _getter = getter; }
	Property(std::function<void(T)> setter) { _setter = setter; }
	Property(std::function<T()> getter, std::function<void(T)> setter) { _getter = getter; _setter = setter; }
	operator T() { return _getter(); }
	void operator= (const T variable) { _setter(variable); }
	//void operator= (const T variable) { _variable = variable; }
	void operator+= (const T variable) { _variable = _variable + variable; }
	T Get() const { return _getter(); }


private:
	T _variable;
	
	std::function<T()> _getter = [&]() -> T { return _variable; };
	std::function<void(T)> _setter = [&](T value) -> void { _variable = value; };
};

