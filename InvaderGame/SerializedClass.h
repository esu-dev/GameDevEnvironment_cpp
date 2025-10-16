#pragma once

#define SERIALIZE_FIELD(v) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeField(#v, v); }

#define SERIALIZE_VECTOR(v, typeData) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeVector(#v, v, typeData); }

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

#define DESERIALIZE_FIELD(v) \
	[&](InstanceData* instanceData) -> void { DeserializeProperty(v, instanceData); }

#define DESERIALIZE_VECTOR(v) \
	[&](InstanceData* instanceData) -> void { DeserializeVector(v, instanceData); }

#define DESERIALIZE(...) \
	void Deserialize(std::vector<std::string> instanceDataVector) override \
	{ \
		std::vector<std::function<void(InstanceData*)>> functionVector = {__VA_ARGS__}; \
		InputValue(instanceDataVector, functionVector); \
	}


#include "framework.h"
#include "SceneDataManager.h"

class Component;

class SerializedClass
{
public:
	virtual std::vector<std::string> Serialize() { return { "not overrided" }; }
	virtual void Deserialize(std::vector<std::string> instanceDataVector) {}


protected:
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

		// Tの型によって処理を分ける
		// templateの部分特殊化を行うことで、検出できるようだが、ここでは一旦パスする
		// いつか実装する
		
		// vector
		if (typeData->isVector)
		{
			serializedDataVector.push_back(name + ":\n");
			// vector<T>の型が分からなくて詰む
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
				serializedDataVector.push_back("- (instanceID)" + value[i].get()->instanceID);
			}
		}
		return serializedDataVector;
	}


	struct InstanceData
	{
		bool isVector = false;
		bool hasInstanceID = false;
		std::vector<std::string> memberVector;
	};
	
	template <typename T>
	static void DeserializeProperty(T& variable, const InstanceData* instanceData)
	{
		for (auto member : instanceData->memberVector)
		{
			if (instanceData->hasInstanceID)
			{
				Object* object = SceneDataManager::GetInstanceID2PointerMap()[member];

				// Tにキャストで良いのでは？

				// 型チェック
				if (Component* component = dynamic_cast<Component*>(object))
				{
				}
				// GameObject
				else if (GameObject* gameObject = dynamic_cast<GameObject*>(object))
				{
					variable = gameObject;
				}

			}
		}
	}

	template <typename T>
	static void DeserializeVector(T& variable, const InstanceData* instanceData)
	{
		for (auto member : instanceData->memberVector)
		{
			if (instanceData->hasInstanceID)
			{
				// インスタンスの検索
				Object* object = SceneDataManager::GetInstanceID2PointerMap()[member];

				// 型チェック
				Component* component = dynamic_cast<Component*>(object);
				if (component != nullptr)
				{
					variable.push_back(std::shared_ptr<Component>(component));
				}
				else
				{
					// GameObject
				}
			}
		}
	}

	static void InputValue(std::vector<std::string> instanceDataVector, std::vector<std::function<void(InstanceData*)>> functionVector)
	{
		bool isPacking = false;
		std::vector<InstanceData*> subInstanceDataVector;
		for (std::string instanceData : instanceDataVector)
		{
			std::smatch m;

			if (isPacking)
			{
				// リスト
				if (std::regex_match(instanceData, m, std::regex(R"(-\s(\(\w+\))(\w+))")))
				{
					subInstanceDataVector.back()->isVector = true;

					// instanceIDをもつかどうか
					if (m[1].str() == "(instanceID)")
					{
						std::string instanceID = m[2].str();
						subInstanceDataVector.back()->hasInstanceID = true;
						subInstanceDataVector.back()->memberVector.push_back(instanceID);
					}
					else
					{

					}
				}
				// クラス, 構造体
				else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(\w+):\s(\w+))")))
				{
					// リストに格納
				}
			}


			std::regex re(R"(^(\w+):(\s*)(.*))");
			if (std::regex_search(instanceData, m, re))
			{
				subInstanceDataVector.push_back(new InstanceData());

				// クラス, 構造体, vector
				if (m[3].str() == "")
				{
					isPacking = true;
				}
				// 値
				else
				{
					isPacking = false;

					// ポインタかどうか確認
					std::smatch match;
					std::string matchString = m[3].str();
					if (std::regex_match(matchString, match, std::regex(R"(\(\w+\)(\w+))")))
					{
						std::string instanceID = match[1].str();
						subInstanceDataVector.back()->hasInstanceID = true;
						subInstanceDataVector.back()->memberVector.push_back(instanceID);
					}
				}
			}
		}


		// データを元に値を代入
		int i = 0;
		for (auto function : functionVector)
		{
			function(subInstanceDataVector[i++]);
		}
	}
};
