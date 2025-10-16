#include "Random.h"


int Random::Range(int min, int max)
{
	// ’x‰„‰Šú‰»
	if (_seed == 0)
	{
		_seed = 10; // Œã‚ÅŠÔ‚Æ‚·‚é
		generator = std::mt19937(_seed);
	}
	std::uniform_int_distribution range(min, max);
	return range(generator);
}