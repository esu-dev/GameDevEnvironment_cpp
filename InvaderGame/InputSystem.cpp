#include "InputSystem.h"

#include "GameEngine.h"


void InputSystem::Update()
{
	for (KeyActionSet& keyActionSet : _keyActionSetVector)
	{
		bool isHolding = false;

		for (KeySet& keySet : keyActionSet.keySetVector)
		{
			if (keySet.isHold)
			{
				if (Input::GetKey(keySet.vkey))
				{
					isHolding = true;
				}
			}
			else
			{
				// GetKeyDown‚ðŒÄ‚Ô‚±‚Æ‚Å—š—ð‚ðƒŠƒZƒbƒg
				if (Input::GetKeyDown(keySet.vkey) && isHolding) keyActionSet.action();
			}
		}
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
