#pragma once

#define SERIALIZE_FIELD(v) \
	[&]() -> void { serializedData += "  " + std::string(#v) + ": " + std::to_string(v) + "\n"; } \

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

class SerializedClass
{
public:
	virtual std::string Serialize() { return ""; }
};