#pragma once

#define SERIALIZE_FIELD(v) \
	[&]() -> void { serializedData += SerializedClass::SerializeField(#v, v); } \

#define SERIALIZE(...) \
	std::string Serialize() override \
	{ \
		std::string serializedData = ""; \
		std::vector<std::function<void()>> functionVector = {__VA_ARGS__}; \
		for (auto& function : functionVector) \
		{ \
			function(); \
		} \
		return serializedData; \
	}


#include "framework.h"
#include "SceneDataManager.h"

class SerializedClass
{
public:
	template <typename T>
	static std::string SerializeField(const std::string& name, const T& value)
	{
		return "  " + name + ": " + std::to_string(value) + "\n";
	}

	virtual std::string Serialize() { return ""; }
	virtual void Deserialize(std::vector<std::string> instanceDataVector) {}
};
