#pragma once

#include "framework.h"

class InputSystem
{
public:
	struct KeySet
	{
		int vkey;
		bool isHold;
	};

	static void Update();

	static void AddKeyAction(std::vector<KeySet> keySetVector, const std::function<void()> action);


private:
	struct KeyActionSet
	{
		std::vector<KeySet> keySetVector;
		std::function<void()> action;
	};

	static std::vector<KeyActionSet> _keyActionSetVector;
};

