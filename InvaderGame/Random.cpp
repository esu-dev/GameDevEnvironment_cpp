#include "Random.h"


int Random::Range(int min, int max)
{
	std::uniform_int_distribution range(min, max);
	return range(generator);
}

int Random::Generate()
{
	std::uniform_int_distribution distribution;
	return distribution(generator);
}

int Random::_seed = 10;
std::mt19937 Random::generator = std::mt19937(Random::_seed);