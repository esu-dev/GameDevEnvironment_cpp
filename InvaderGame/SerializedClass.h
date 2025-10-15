#pragma once

#define SERIALIZE_FIELD(v) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeField(#v, v); } \

#define SERIALIZE_VECTOR(v, typeData) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeVector(#v, v, typeData); } \

#define SERIALIZE(...) \
	std::vector<std::string> Serialize() override \
	{ \
		std::vector<std::string> r; \
		std::vector<std::function<std::vector<std::string>()>> functionVector = {__VA_ARGS__}; \
		for (auto& function : functionVector) \
		{ \
			std::vector<std::string> s = function(); \
			r.insert(r.end(), s.begin(), s.end()); \
		} \
		return r; \
	}


#include "framework.h"
#include "SceneDataManager.h"

class SerializedClass
{
public:
	struct TypeData
	{
		bool isVector;
		bool isPointer;
		bool isSmartPointer;

		TypeData(bool isVector, bool isPointer, bool isSmartPointer)
		{
			this->isVector = isVector;
			this->isPointer = isPointer;
			this->isSmartPointer = isSmartPointer;
		}
	};

	template <typename T>
	static std::vector<std::string> SerializeField(const std::string& name, const T& value, const TypeData* typeData = new TypeData(false, false, false))
	{
		std::vector<std::string> serializedDataVector;

		// T‚ÌŒ^‚É‚æ‚Á‚Äˆ—‚ð•ª‚¯‚é
		// template‚Ì•”•ª“ÁŽê‰»‚ðs‚¤‚±‚Æ‚ÅAŒŸo‚Å‚«‚é‚æ‚¤‚¾‚ªA‚±‚±‚Å‚Íˆê’UƒpƒX‚·‚é
		// ‚¢‚Â‚©ŽÀ‘•‚·‚é
		
		// vector
		if (typeData->isVector)
		{
			serializedDataVector.push_back(name + ":\n");
			// vector<T>‚ÌŒ^‚ª•ª‚©‚ç‚È‚­‚Ä‹l‚Þ
			/*for (int i = 0; i < value.size(); i++)
			{
				if (typeData->isSmartPointer)
				{
					serializedDataVector.push_back("  " + value[i].get()->instanceID);
				}
			}*/
		}

		return serializedDataVector;
	}

	template <typename T>
	static std::vector<std::string> SerializeVector(const std::string& name, const std::vector<T> value, const TypeData* typeData = new TypeData(false, false, false))
	{
		std::vector<std::string> serializedDataVector;
		serializedDataVector.push_back(name + ":");
		for (int i = 0; i < value.size(); i++)
		{
			if (typeData->isSmartPointer)
			{
				serializedDataVector.push_back("  " + value[i].get()->instanceID);
			}
		}
		return serializedDataVector;
	}

	virtual std::vector<std::string> Serialize() { return {}; }
	virtual void Deserialize(std::vector<std::string> instanceDataVector) {}
};
