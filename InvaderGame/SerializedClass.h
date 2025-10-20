#pragma once

#define SERIALIZE_FIELD(v) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeField(#v, v, indentNum); }

#define SERIALIZE_VECTOR(v) \
	[&]() -> std::vector<std::string> { return SerializedClass::SerializeVector(#v, v); }

#define SERIALIZE(p, ...) \
	std::vector<std::string> Serialize(const int indentNum = 1) override \
	{ \
		std::vector<std::string> pV = p::Serialize(indentNum); \
		std::vector<std::string> r; \
		if (pV[0] != "") r.insert(r.end(), pV.begin(), pV.end()); \
		std::vector<std::function<std::vector<std::string>()>> functionVector = {__VA_ARGS__}; \
		for (auto& function : functionVector) \
		{ \
			std::vector<std::string> s = function(); \
			r.insert(r.end(), s.begin(), s.end()); \
		} \
		return r; \
	}

#define DESERIALIZE_FIELD(v) \
	[&](InstanceData* instanceData) -> void { DeserializeField(v, instanceData); }

#define DESERIALIZE_POINTER(v) \
	[&](InstanceData* instanceData) -> void { DeserializePointer(v, instanceData); }

#define DESERIALIZE_VECTOR(v) \
	[&](InstanceData* instanceData) -> void { DeserializeVector(v, instanceData); }

// out引数で返した方がシリアライズとも統一感があってよいのでは？
#define DESERIALIZE(p, ...) \
	int Deserialize(std::vector<std::string> v) override \
	{ \
		int n = p::Deserialize(v); \
		std::vector<std::function<void(InstanceData*)>> fv = {__VA_ARGS__}; \
		if (n >= 2) v.erase(v.begin(), v.begin() + n - 1); \
		else if (n == 1) v.erase(v.begin()); \
		InputValue(v, fv); \
		return n + (int)fv.size(); \
	}

#define SERIALIZE_FIELD2(v) \
	std::pair<std::function<std::vector<std::string>(int)>, std::function<void(InstanceData*)>>( \
		[&](int indentNum) -> std::vector<std::string> { return SerializedClass::SerializeField(#v, v, indentNum); }, \
		[&](InstanceData* instanceData) -> void { DeserializeField(v, instanceData); } \
	)

#define SERIALIZE2(p, ...) \
	std::vector<std::string> Serialize(const int indentNum = 1) override \
	{ \
		std::vector<std::string> pV = p::Serialize(indentNum); \
		std::vector<std::string> r; \
		if (pV[0] != "") r.insert(r.end(), pV.begin(), pV.end()); \
		std::vector<std::pair<std::function<std::vector<std::string>(int)>, std::function<void(InstanceData*)>>> functionVector = {__VA_ARGS__}; \
		for (auto& function : functionVector) \
		{ \
			std::vector<std::string> s = function.first(indentNum); \
			r.insert(r.end(), s.begin(), s.end()); \
		} \
		return r; \
	} \
	\
	int Deserialize(std::vector<std::string> v) override \
	{ \
		int n = p::Deserialize(v); \
		std::vector<std::pair<std::function<std::vector<std::string>(int)>, std::function<void(InstanceData*)>>> fv = {__VA_ARGS__}; \
		if (n >= 2) v.erase(v.begin(), v.begin() + n - 1); \
		else if (n == 1) v.erase(v.begin()); \
		InputValue2(v, fv); \
		return n + (int)fv.size(); \
	}



#include "framework.h"
#include "SceneDataManager.h"

class Component;

class SerializedClass
{
public:
	virtual std::vector<std::string> Serialize(const int indentNum = 1) { return { "" }; }
	virtual int Deserialize(std::vector<std::string> v) { return 0; } // intを返すのは、親クラスの処理数を教えるため


protected:
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
			serializedDataVector.push_back(indent + name + ": " + std::to_string(value));
		}
		// ポインタ
		else if constexpr (std::is_pointer<T>())
		{
			serializedDataVector.push_back(indent + name + ": " + value->instanceID);
		}
		// シリアライズ可能
		else if constexpr (std::is_base_of<SerializedClass, T>())
		{
			serializedDataVector.push_back(indent + name + ":");
			std::vector<std::string> subSerializeDataVector = value.Serialize(indentNum + 1);
			serializedDataVector.insert(serializedDataVector.end(), subSerializeDataVector.begin(), subSerializeDataVector.end());
		}
		// その他はstringとみなす
		// もし例外がある場合は、部分特殊化で実装する
		else
		{
			//serializedDataVector.push_back(indent + name + ": " + std::to_string(value));
			serializedDataVector.push_back(indent + name + ": " + "error");
		}

		return serializedDataVector;
	}

	template <typename T>
	static std::vector<std::string> SerializeVector(const std::string& name, const std::vector<T>& value)
	{
		std::vector<std::string> serializedDataVector;
		serializedDataVector.push_back(name + ":");
		for (int i = 0; i < value.size(); i++)
		{
			serializedDataVector.push_back("- (instanceID)" + value[i].get()->instanceID);
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
	static void DeserializeField(T& variable, const InstanceData* instanceData)
	{
		// constexpr(constant expression)でコンパイル分岐が可能になる
		// 値ならそのまま代入
		if constexpr (std::is_arithmetic<T>())
		{
			std::string value = instanceData->memberVector[0];

			// float
			if (typeid(T) == typeid(float))
			{
				variable = std::stof(value);
			}
			else
			{

			}
		}
		// シリアライズできる場合(ex. Record)
		else if constexpr (std::is_base_of<SerializedClass, T>())
		{
			variable.Deserialize(instanceData->memberVector);
		}
	}

	template <typename T>
	static void DeserializePointer(T& variable, const InstanceData* instanceData)
	{
		for (auto member : instanceData->memberVector)
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

	static void InputValue(const std::vector<std::string>& instanceDataVector, const std::vector<std::function<void(InstanceData*)>>& functionVector)
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
				else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(\s*\w+:\s*.*))")))
				{
					// リストの格納
					subInstanceDataVector.back()->memberVector.push_back(m[1].str());
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
				// 値, ポインタ
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
					else
					{
						subInstanceDataVector.back()->memberVector.push_back(matchString);
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

	static void InputValue2(const std::vector<std::string>& instanceDataVector, const std::vector<std::pair<std::function<std::vector<std::string>(int)>, std::function<void(InstanceData*)>>>& functionVector)
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
				else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(\s*\w+:\s*.*))")))
				{
					// リストの格納
					subInstanceDataVector.back()->memberVector.push_back(m[1].str());
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
				// 値, ポインタ
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
					else
					{
						subInstanceDataVector.back()->memberVector.push_back(matchString);
					}
				}
			}
		}


		// データを元に値を代入
		int i = 0;
		for (auto& function : functionVector)
		{
			function.second(subInstanceDataVector[i++]);
		}
	}
};
