#pragma once

#include "EngineFramework.h"

class std_extension
{
public:
	template <typename T1, typename T2> static std::vector<T2> Select(const std::vector<T1>& vector, const std::function<T2(T1)>& function)
	{
		std::vector<T2> result;
		std::transform(vector.begin(), vector.end(), std::back_inserter(result), function);

		return result;
	}

	template <typename T> static T Find(const std::vector<T>& vector, const std::function<bool(T)>& function)
	{
		auto iterator = std::find_if(vector.begin(), vector.end(), function);
		if (iterator == vector.end())
		{
			return nullptr;
		}
		__int64 index = std::distance(vector.begin(), iterator);
		return vector[index];
	}

	template <typename T>
	static void Remove(std::vector<T>& vector, T element)
	{
		vector.erase(std::remove(vector.begin(), vector.end(), element), vector.end());
	}

	// テンプレート構造体
	template <typename T>
	struct is_vector : std::false_type {};

	// テンプレート部分特殊化
	template <typename T>
	struct is_vector<std::vector<T>> : std::true_type {};

	template <typename T>
	static constexpr bool is_vector_v = is_vector<T>::value;


	template <typename T>
	struct is_string : std::false_type {};

	template <>
	struct is_string<std::string> : std::true_type {};

	template <typename T>
	static constexpr bool is_string_v = is_string<T>::value;

	
	template <typename T>
	struct is_shared_ptr : std::false_type {};

	template <typename U>
	struct is_shared_ptr<std::shared_ptr<U>> : std::true_type {};

	template <typename U>
	static constexpr bool is_shared_ptr_v = is_shared_ptr<U>::value;


	static std::string StringVector2String(const std::vector<std::string>& stringVector);
};
