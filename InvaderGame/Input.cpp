#include "Input.h"

void Input::Update()
{
	int length = sizeof(_isVkeyDown) / sizeof(bool);
	for (int i = 0; i < length; i++)
	{
		_isVkeyDown[i] = false;
		if (GetAsyncKeyState(i) & 0x0001)
		{
			_isVkeyDown[i] = true;
		}
	}
}

bool Input::GetKey(int vkey)
{
	return GetAsyncKeyState(vkey);
}

bool Input::GetKeyDown(int vkey)
{
	return _isVkeyDown[vkey];
}

bool Input::AnyKeyDown()
{
	for (bool isDown : _isVkeyDown)
	{
		if (isDown) return true;
	}
	return false;
}

bool Input::_isVkeyDown[150] = {};