#pragma once

template <typename T>
class PropertyBase
{
public:
	PropertyBase() {}
	PropertyBase(std::function<T()> get) { _get = get; }
	operator T() { return _get(); }


protected:
	T _variable;


private:
	std::function<T()> _get = [&]() -> T { return _variable; };
};