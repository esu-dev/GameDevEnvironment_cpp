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
	//Property(std::function<T()> get, std::function<void(T)> set) { _get = get; _set = set; }
	operator T() { return _get(); }
	//void operator= (const T variable) { _set(variable); }
	void operator= (const T variable) { _variable = variable; }


private:
	T _variable;
	
	std::function<T()> _get = [&]() -> T { return _variable; };
	//std::function<int()> get = []() -> int { return 0; };
	//std::function<void(T)> _set = [&](T value) -> void { _variable = value; };
};

