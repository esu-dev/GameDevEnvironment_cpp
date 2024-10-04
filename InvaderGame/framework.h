// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する

// Windows ヘッダー ファイル
#include <windows.h>
#include <WinUser.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <box2d.h>

#include <string>
#include <vector>
#include <list>

#include <functional>
#include <algorithm>
#include <iterator>

#include "Debug.h"