#include "InputSystem.h"

#include "GameEngine.h"


void InputSystem::Update()
{
	for (KeyActionSet keyActionSet : _keyActionSetVector)
	{
		for (int vkey : keyActionSet.vkeyVector)
		{
			if (!Input::GetKeyDown(vkey))
			{
				return;
			}
		}

		keyActionSet.action();
	}
}

template<typename... vkey>
void InputSystem::AddKeyAction(const std::function<void()> action, const vkey... vkeys)
{
	
}


std::vector<InputSystem::KeyActionSet> InputSystem::_keyActionSetVector;
