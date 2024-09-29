#include "Event.h"

void Event::AddListener(std::function<void(void)> func)
{
	_functionVector.push_back(func);
}

void Event::Invoke()
{
	for (auto func : _functionVector)
	{
		func();
	}
}
