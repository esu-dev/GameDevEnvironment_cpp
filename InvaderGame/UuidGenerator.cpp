#include "UuidGenerator.h"

#include "Utility.h"

std::string UuidGenerator::Generate()
{
	int r1 = Random::Generate();
	int r2 = Random::Generate();
	int r3 = Random::Generate();
	return std::to_string(r1) + "=" + std::to_string(r2) + "=" + std::to_string(r3);
}