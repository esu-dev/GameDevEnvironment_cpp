#pragma once

// ファイル読み書き関係
//#include <fstream> // なぜかこいつが使えない
//#include <iostream>

//#include <stdlib.h>
//#include <string>

#include "framework.h"

class FileReader
{
public:
	static void Read(std::vector<std::string>& outContentVector, std::string path);
};

