#pragma once

#include "framework.h"

class StringConverter
{
public:
	static LPWSTR stringToLPWSTR(std::string str)
	{
		setlocale(LC_CTYPE, "jpn"); // 地域設定を日本語に設定。日本語はマルチバイト文字であるから変換する必要がある。
		wchar_t wide[256]; // wchar_tはワイド文字（ユニコード）を表すためのデータ型
		size_t ret; // size_t はオブジェクトのバイト数を表現できる程度に十分に大きい符号なし整数型。
		mbstowcs_s(&ret, wide, str.c_str(), 256);
		return wide;
	}
};