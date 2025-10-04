#include "InputSystem.h"

#include "GameEngine.h"


void InputSystem::Update()
{
	for (KeyActionSet keyActionSet : _keyActionSetVector)
	{
		for (KeySet keySet : keyActionSet.keySetVector)
		{
			if (keySet.isHold)
			{
				if (!Input::GetKey(keySet.vkey))
				{
					return;
				}
			}
			else
			{
				if (!Input::GetKeyDown(keySet.vkey))
				{
					return;
				}
			}
		}

		keyActionSet.action();
	}
}

void InputSystem::AddKeyAction(std::vector<KeySet> keySetVector, const std::function<void()> action)
{
	KeyActionSet keyActionSet;
	keyActionSet.keySetVector = keySetVector;
	keyActionSet.action = action;
}


std::vector<InputSystem::KeyActionSet> InputSystem::_keyActionSetVector;
