#pragma once

#include "framework.h"

class Random
{
public:
	int Range(int min, int max);

private:
	static float _seed;
	std::mt19937 generator;
};

