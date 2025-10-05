#pragma once

#include <stdlib.h>
#include <functional>

class Vector2;

template <typename T>
class Property
{
public:
	Property() {}
	Property(std::function<T()> get) { _get = get; }
	operator T() { return _get(); }
	//void operator= (const T variable) { _set(variable); }
	void operator= (const T variable) { _variable = variable; }

	void operator+= (const T variable) { _variable = _variable + variable; }


private:
	T _variable;
	
	std::function<T()> _get = [&]() -> T { return _variable; };
	//std::function<void(T)> _set = [&](T value) -> void { _variable = value; };
};

