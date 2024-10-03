#include "Input.h"

bool Input::GetKey(int vkey)
{
	return GetAsyncKeyState(vkey) & 0x8000;
}

bool Input::GetKeyDown(int vkey)
{
	return GetAsyncKeyState(vkey) & 0x0001;
}