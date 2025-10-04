#include "InputSystem.h"

#include "GameEngine.h"


void InputSystem::Update()
{
	for (KeyActionSet& keyActionSet : _keyActionSetVector)
	{
		// “¯Žž‰Ÿ‚µ‚¶‚á‚È‚­‚Ä‚à”½‰ž‚µ‚Ä‚µ‚Ü‚¤ƒoƒO‚ª‚ ‚é‚ªˆê’U•ú’u
		for (KeySet& keySet : keyActionSet.keySetVector)
		{
			if (keySet.isHold)
			{
				if (!Input::GetKey(keySet.vkey))
				{
					goto CONT;
				}
			}
			else
			{
				if (!Input::GetKeyDown(keySet.vkey))
				{
					goto CONT;
				}
			}
		}

		keyActionSet.action();

	CONT:;
	}
}

void InputSystem::AddKeyAction(std::vector<KeySet> keySetVector, const std::function<void()> action)
{
	KeyActionSet keyActionSet;
	keyActionSet.keySetVector = keySetVector;
	keyActionSet.action = action;

	_keyActionSetVector.push_back(keyActionSet);
}


std::vector<InputSystem::KeyActionSet> InputSystem::_keyActionSetVector;
