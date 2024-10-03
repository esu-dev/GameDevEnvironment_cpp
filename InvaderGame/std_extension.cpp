#include "std_extension.h"

// ‚â‚Á‚Ä‚é‚±‚Æ“¯‚¶‚¶‚á‚Ë
template <typename T1, typename T2> T1 std_extension::Select(std::vector<T1> vector, std::function<T2(T1)> function)
{
	std::vector<T1> result;
	std::transform(vector.begin(), vector.end(), std::back_inserter(result), function);
}