#pragma once

#include <stdlib.h>
#include <functional>

template <typename T>
class Property
{
public:
	Property() {}
	//Property(std::function<T> get, std::function<void(T)> set);
	//operator T() { return get(); }
	//void operator= (const T variable) { set(variable); }


private:
	T _variable;
	
	//std::function<T> get = []() -> T { return _variable; };
	//std::function<void(T)> set = [](T value) -> void { _variable = value; };
};

