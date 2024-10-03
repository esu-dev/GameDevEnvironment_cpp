#pragma once

#include "framework.h"

class std_extension
{
public:
	template <typename T1, typename T2> T1 Select(std::vector<T1> vector, std::function<T2(T1)> function);
};