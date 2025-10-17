#pragma once

#include "framework.h"

class Random
{
public:
	static int Range(int min, int max);
	static int Generate();

private:
	static int _seed;
	static std::mt19937 generator;
};

