#include "std_extension.h"

std::string std_extension::StringVector2String(const std::vector<std::string>& stringVector)
{
	std::string str;
	for (std::string line : stringVector)
	{
		str += line + "\n";
	}
	return str;
}