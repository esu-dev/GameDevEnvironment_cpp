// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

// Windows標準のライブラリをまとめてインクルードする

#pragma once

// mmio(multimedia IO)に関するライブラリを追加
#pragma comment(lib, "winmm.lib")

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する

// Windows ヘッダー ファイル
#include <windows.h>
#include <WinUser.h>

// mmio(multimedia IO)に関するライブラリをインクルード
#include <mmsystem.h>


// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <locale.h>
#include <box2d.h>

#include <string>
#include <vector>
#include <list>

#include <functional>
#include <algorithm>
#include <iterator>

#include "Debug.h"
#include "std_extension.h"
#include "AudioClip.h"