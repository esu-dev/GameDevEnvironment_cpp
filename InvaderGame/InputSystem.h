#pragma once

#include "EngineFramework.h"

class InputSystem
{
public:
	struct KeySet
	{
		int vkey = 0;
		bool isHold = false;

		KeySet() {}
		KeySet(int vkey) { this->vkey = vkey; }
		KeySet(int vkey, bool isHold) { this->vkey = vkey; this->isHold = isHold; }
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

