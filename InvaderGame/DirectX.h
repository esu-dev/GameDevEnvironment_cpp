#pragma once

// Direct3Dのライブラリを使用できるようにする
// オブジェクトファイルに、リンカでリンクするライブラリの名前を記述するもの。
// オブジェクトファイルとは、アセンブラによって生成される機械語のファイルのこと
// 複数のオブジェクトファイルを１つにまとめることをリンカという
// リンクを実行するのがリンカー
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")

// Direct3Dの型・クラス・関数などを呼べるようにする
#include <d3d11.h>
#include <d3dcompiler.h>

// DirectXMath(数学ライブラリ)を使用できるようにする
#include <DirectXMath.h>

// DirectXテクスチャライブラリを使用できるようにする
#include <DirectXTex.h>

// ComPtrを使用できるようにする
// ComPtrはスマートポインタ。ポインタをラップしている
// ポインタに関連づけられたオブジェクトの管理をするために使う
// COM(Component Object Model)
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

// 自作のDirectX関係のヘッダーをインクルード
#include "Direct3D.h"
#include "Texture.h"
