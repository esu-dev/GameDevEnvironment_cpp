#include "Input.h"

bool Input::GetKey(int vkey)
{
	return GetAsyncKeyState(vkey);
}

// 前回の履歴が残るバグあり？
// 自力で前フレームを比較する必要あり
bool Input::GetKeyDown(int vkey)
{
	// ここで２度呼んで履歴をリセットしても良いかも
	return GetAsyncKeyState(vkey) & 0x0001;
}