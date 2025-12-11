#pragma once

#define SERIALIZE_FIELD3(v) \
	std::make_shared<SerializeFuncData>( \
		[&](int indentNum) -> std::vector<std::string> { return SerializedClass::SerializeField(#v, v, indentNum); }, \
		[&](InstanceData* instanceData) -> void { DeserializeField(v, instanceData); }, \
		[&]() -> FieldInfo { return FieldInfo(#v, typeid(v).name()); }, \
		[&](std::string name) -> void { }) \

#define SERIALIZE3(p, ...) \
	std::vector<std::shared_ptr<SerializeFuncData>> GetSerializeFuncData() override \
	{ \
		std::vector<std::shared_ptr<SerializeFuncData>> pV = p::GetSerializeFuncData(); \
		std::vector<std::shared_ptr<SerializeFuncData>> sfdV = { __VA_ARGS__ }; \
		pV.insert(pV.end(), sfdV.begin(), sfdV.end()); \
		return pV; \
	} \
	\
	std::vector<std::string> Serialize(const int indentNum = 1) override \
	{ \
		std::vector<std::string> r; \
		std::vector<std::shared_ptr<SerializeFuncData>> sfdV = GetSerializeFuncData(); \
		for (auto& sfd : sfdV) \
		{ \
			std::vector<std::string> s = sfd->serializeFunc(indentNum); \
			r.insert(r.end(), s.begin(), s.end()); \
		} \
		return r; \
	} \
	\
	int Deserialize(std::vector<std::string> v) override \
	{ \
		std::vector<std::shared_ptr<SerializeFuncData>> sfdV = GetSerializeFuncData(); \
		InputValue3(v, sfdV); \
		return 0; \
	}



#include "framework.h"
#include "std_extension.h"
#include "Debug.h"
#include "SceneDataManager.h"
#include "AssetManager.h"

class Component;

class SerializedClass
{
public:
	struct FieldInfo
	{
		std::string name;
		std::string type;

		FieldInfo(std::string name, std::string type)
		{
			this->name = name;
			this->type = type;
		}
	};
	
	virtual std::vector<std::string> Serialize(const int indentNum = 1) { return { "" }; }
	virtual int Deserialize(std::vector<std::string> v) { return 0; } // intを返すのは、親クラスの処理数を教えるため

	SerializedClass() {}


protected:
	struct InstanceData
	{
		std::string variableName;
		std::vector<std::string> memberVector;
	};

	struct SerializeFuncData
	{
		std::function<std::vector<std::string>(int)> serializeFunc;
		std::function<void(InstanceData*)> deserializeFunc;
		std::function<FieldInfo()> getFieldFunc;
		std::function<void(std::string)> setFieldFunc;

		SerializeFuncData(
			std::function<std::vector<std::string>(int)> serilizeFunc,
			std::function<void(InstanceData*)> deserializeFunc,
			std::function<FieldInfo()> getFieldFunc,
			std::function<void(std::string)> setFieldFunc)
		{
			this->serializeFunc = serilizeFunc;
			this->deserializeFunc = deserializeFunc;
			this->getFieldFunc = getFieldFunc;
			this->setFieldFunc = setFieldFunc;
		}
	};

	template <typename T>
	static std::vector<std::string> SerializeField(const std::string& name, T& value, const int indentNum = 1)
	{
		std::vector<std::string> serializedDataVector;

		std::string indent = "";
		for (int i = 0; i < indentNum; i++)
		{
			indent += "  ";
		}

		// 値
		if constexpr (std::is_arithmetic<T>())
		{
			if (typeid(T) == typeid(bool))
			{
				serializedDataVector.push_back(indent + name + ": " + (std::to_string(value) == "0" ? "false" : "true"));
			}
			else
			{
				serializedDataVector.push_back(indent + name + ": " + std::to_string(value));
			}
		}
		// ポインタ
		else if constexpr (std::is_pointer<T>())
		{
			if (value == nullptr) serializedDataVector.push_back(indent + name + ": (instanceID)nullptr");
			else serializedDataVector.push_back(indent + name + ": (instanceID)" + value->instanceID);
		}
		// シリアライズ可能
		else if constexpr (std::is_base_of<SerializedClass, T>())
		{
			serializedDataVector.push_back(indent + name + ":");
			std::vector<std::string> subSerializeDataVector = value.Serialize(indentNum + 1);
			serializedDataVector.insert(serializedDataVector.end(), subSerializeDataVector.begin(), subSerializeDataVector.end());
		}
		// vector
		else if constexpr (std_extension::is_vector_v<T>)
		{
			serializedDataVector.push_back(indent + "(vector)" + name + ": " + std::to_string(value.size()));
			for (int i = 0; i < value.size(); i++)
			{
				// 値
				if constexpr (std::is_arithmetic<typename T::value_type>())
				{
					serializedDataVector.push_back(indent + "- " + std::to_string(value[i]));
				}
				// 要素がポインタ
				else if constexpr (std::is_pointer<typename T::value_type>())
				{
					if (value[i] == nullptr)
					{
						serializedDataVector.push_back(indent + "- (instanceID)nullptr");
					}
					else
					{
						serializedDataVector.push_back(indent + "- (instanceID)" + value[i]->instanceID);
					}
				}
				// shardPtr
				else if constexpr (std_extension::is_shared_ptr_v<typename T::value_type>)
				{
					serializedDataVector.push_back(indent + "- (instanceID)" + value[i].get()->instanceID);
				}
				// シリアライズ可能
				else if constexpr (std::is_base_of<SerializedClass, typename T::value_type>())
				{
					// シリアライズ
					std::vector<std::string> subSerializedDataVec = value[i].Serialize();

					// 結果をvectorに結合
					serializedDataVector.push_back(indent + "- " + std::to_string(i) + ":");
					for (int i = 0; i < subSerializedDataVec.size(); i++)
					{
						serializedDataVector.push_back(indent + "  " + subSerializedDataVec[i]);
					}
				}
			}
		}
		// 文字列
		else if constexpr (std_extension::is_string_v<T>)
		{
			serializedDataVector.push_back(indent + name + ": " + value);
		}
		else
		{
			serializedDataVector.push_back(indent + name + ": " + "error");
		}

		return serializedDataVector;
	}
	
	template <typename T>
	static void DeserializeField(T& variable, const InstanceData* instanceData)
	{
		auto getInstanceFromID = [](std::string instanceID) -> Object* {
			Object* object = nullptr;
			// シーンから検索
			if (SceneDataManager::GetInstanceID2PointerMap().find(instanceID) != SceneDataManager::GetInstanceID2PointerMap().end())
			{
				object = SceneDataManager::GetInstanceID2PointerMap()[instanceID];
			}
			// アセットから検索
			else if (AssetManager::GetInstanceID2PointerMap().find(instanceID) != AssetManager::GetInstanceID2PointerMap().end())
			{
				object = AssetManager::GetInstanceID2PointerMap()[instanceID];
			}

			if (object == nullptr)
			{
				Debug::Log("存在しないインスタンスでデシリアライズしようとしています。[DeserializeField()]");
			}

			return object;
		};

		// constexpr(constant expression)でコンパイル分岐が可能になる
		// 値ならそのまま代入
		if constexpr (std::is_arithmetic<T>())
		{
			std::string value = instanceData->memberVector[0];

			// intを追加すると警告が出る
			// float
			if (typeid(T) == typeid(float))
			{
				variable = std::stof(value);
			}
			else if (typeid(T) == typeid(bool))
			{
				variable = (value == "true");
			}
			else
			{
				Debug::Log("型を追加してください。[DeserializeField()]");
			}
		}
		// ポインタ
		else if constexpr (std::is_pointer<T>())
		{
			if (instanceData->memberVector[0] == "nullptr")
			{
				variable = nullptr;
				return;
			}

			Object* object = getInstanceFromID(instanceData->memberVector[0]);

			if (object == nullptr)
			{
				variable = nullptr;
			}
			else if (T t = dynamic_cast<T>(object))
			{
				variable = t;
			}
		}
		// シリアライズできる場合(ex. Record)
		else if constexpr (std::is_base_of<SerializedClass, T>())
		{
			variable.Deserialize(instanceData->memberVector);
		}
		// vector
		else if constexpr (std_extension::is_vector_v<T>)
		{
			variable.clear();

			// 要素ごとに分離
			std::vector<std::vector<std::string>> elementVector;
			int index = -1;

			for (int i = 0; i < std::stoi(instanceData->memberVector[0]); i++)
			{
				elementVector.push_back(std::vector<std::string>());
			}

			// elementVectorの設定
			for (int i = 1; i < instanceData->memberVector.size(); i++)
			{
				std::smatch smatch;

				// 要素が値
				if (std::regex_match(instanceData->memberVector[i], smatch, std::regex(R"([^:]+)")))
				{
					//elementVector.push_back(std::vector<std::string>());
					index++;
					if (index >= elementVector.size())
					{
						break;
					}
					elementVector[index].push_back(instanceData->memberVector[i]);
				}
				// 要素がポインタ
				else if (std::regex_match(instanceData->memberVector[i], smatch, std::regex(R"([^\s]+:\s(\(\w+\).+))")))
				{
					index++;
					if (index >= elementVector.size())
					{
						break;
					}
					elementVector[index].push_back(smatch[1].str());
				}
				// 要素がクラス、構造体
				else if (std::regex_match(instanceData->memberVector[i], smatch, std::regex(R"(\d+:)")))
				{
					//elementVector.push_back(std::vector<std::string>());
					index++;
					if (index >= elementVector.size())
					{
						break;
					}
				}
				// 要素がシリアライズ可能なオブジェクトのメンバー
				else if (std::regex_match(instanceData->memberVector[i], smatch, std::regex(R"(\s{2}(.+:.*))")))
				{
					elementVector[index].push_back(smatch[1]);
				}
				else
				{
					Debug::Log("型を追加してください。[DeserializeField()]");
				}
			}


			for (int i = 0; i < elementVector.size(); i++)
			{
				bool isEmpty = (elementVector[i].size() == 0);
				std::string member = "";
				if (!isEmpty)
				{
					member = elementVector[i][0];
				}

				// 要素が値
				if constexpr (std::is_arithmetic<typename T::value_type>())
				{
					// int
					if (typeid(typename T::value_type) == typeid(int))
					{
						variable.push_back(isEmpty ? 0 : std::stoi(member));
					}
					// float
					else if (typeid(typename T::value_type) == typeid(float))
					{
						variable.push_back(isEmpty ? 0 : std::stof(member));
					}
					else
					{
						Debug::Log("型を追加してください。[DeserializeField()]");
					}
				}
				// 要素がポインタ
				else if constexpr (std::is_pointer<typename T::value_type>())
				{
					if (isEmpty)
					{
						variable.push_back(nullptr);
						continue;
					}

					if (member == "nullptr")
					{
						variable.push_back(nullptr);
						continue;
					}

					// インスタンスの検索
					Object* object = getInstanceFromID(member);

					variable.push_back((typename T::value_type)(object));
				}
				// shared_ptr
				else if constexpr (std_extension::is_shared_ptr_v<typename T::value_type>)
				{
					// インスタンスの検索
					Object* object = getInstanceFromID(member);

					if (Component* component = dynamic_cast<Component*>(object)) variable.push_back(std::shared_ptr<Component>(component));
				}
				// シリアライズ可能
				else if constexpr (std::is_base_of<SerializedClass, typename T::value_type>())
				{
					auto serializedObject = typename T::value_type();
					
					if (!isEmpty)
					{
						serializedObject.Deserialize(elementVector[i]);
					}

					variable.push_back(serializedObject);
				}
			}
		}
		// 文字列
		else if constexpr (std_extension::is_string_v<T>)
		{
			variable = instanceData->memberVector[0];
		}
	}
	

	static void InputValue3(const std::vector<std::string>& instanceDataVector, const std::vector<std::shared_ptr<SerializeFuncData>>& functionVector);

	virtual std::vector<std::shared_ptr<SerializeFuncData>> GetSerializeFuncData() { return std::vector<std::shared_ptr<SerializeFuncData>>(); }
};
