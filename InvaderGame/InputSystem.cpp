#include "InputSystem.h"

#include "GameEngine.h"


void InputSystem::Update()
{
	for (KeyActionSet& keyActionSet : _keyActionSetVector)
	{
		bool canAction = true;

		for (KeySet& keySet : keyActionSet.keySetVector)
		{
			if (keySet.isHold)
			{
				if (!Input::GetKey(keySet.vkey))
				{
					canAction = false;
				}
			}
			else
			{
				// GetKeyDownを呼ぶことで履歴をリセット
				// 同じ文字は１フレーム内で１度しか認識されない
				if (!Input::GetKeyDown(keySet.vkey)) canAction = false;
			}
		}

		if (canAction) keyActionSet.action();
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
