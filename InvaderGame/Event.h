#pragma once

#include "framework.h"

class Event
{
public:
	// ‚È‚º‚©.cpp‚ÅŽÀ‘•‚Å‚«‚È‚©‚Á‚½‚½‚ß.h‚ÅŽÀ‘•
	void AddListener(std::function<void(void)> func);
	void Invoke();
	void RemoveAllListener();

private:
	std::vector<std::function<void(void)>> _functionVector;
};