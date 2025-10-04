#pragma once

#include "framework.h"

class InputSystem
{
public:
	static void Update();

	template<typename... vkey>
	static void AddKeyAction(const std::function<void()> action, const vkey... vkeys);


private:
	struct KeyActionSet
	{
		std::vector<int> vkeyVector;
		std::function<void()> action;
	};

	static std::vector<KeyActionSet> _keyActionSetVector;
};

