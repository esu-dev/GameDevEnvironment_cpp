#pragma once

#include "framework.h"

class std_extension
{
public:
	template <typename T1, typename T2> static T1 Select(std::vector<T1> vector, std::function<T2(T1)> function)
	{
		std::vector<T1> result;
		std::transform(vector.begin(), vector.end(), std::back_inserter(result), function);
	}

	template <typename T> static T Find(std::vector<T> vector, std::function<bool(T)> function)
	{
		auto iterator = std::find_if(vector.begin(), vector.end(), function);
		if (iterator == vector.end())
		{
			return nullptr;
		}
		int index = std::distance(vector.begin(), iterator);
		return vector[index];
	}
};
