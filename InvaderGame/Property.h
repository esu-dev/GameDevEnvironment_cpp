#pragma once

#include <stdlib.h>
#include <functional>

class Vector2;

template <typename T>
class Property
{
public:
	Property() {}
	Property(std::function<T()> getter) { _getter = getter; }
	Property(std::function<void(T)> setter) { _setter = setter; }
	operator T() { return _getter(); }
	void operator= (const T variable) { _setter(variable); }
	//void operator= (const T variable) { _variable = variable; }

	void operator+= (const T variable) { _variable = _variable + variable; }


private:
	T _variable;
	
	std::function<T()> _getter = [&]() -> T { return _variable; };
	std::function<void(T)> _setter = [&](T value) -> void { _variable = value; };
};

