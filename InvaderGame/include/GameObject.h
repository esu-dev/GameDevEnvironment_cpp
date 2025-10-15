#pragma once

#include "framework.h"
#include "Utility.h"

#include "Object.h"
#include "Scene.h"
//#include "Component.h"

class Component;
class Transform;
class TextLabel;
class SpriteRenderer;
class Animator;

using namespace SceneManagement;

class GameObject : public Object
{
public:
	static std::vector<GameObject*> FindGameObjectsWithTag(std::string tag);
	static GameObject* Create();

	Scene* scene;
	std::string tag = "Untagged";

	// 関数テンプレートは型やコンパイル時に定まる値をパラメータ化する機能
	// 利用されるときに実体化するため、正しく分割コンパイルされない
	template <typename T> T* AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>();
		component.get()->gameObject = this;
		if (this->_transform != nullptr) ((Component*)component.get())->SetTransform(this->_transform);
		_componentVector.push_back(component);
		return component.get();
	}

	template <typename T> T* GetComponent()
	{
		auto iterator = std::find_if(_componentVector.begin(), _componentVector.end(), [](std::shared_ptr<Component> x)
			{ 
				return typeid(*x) == typeid(T) || dynamic_cast<T*>(x.get());
			});
		if (iterator == _componentVector.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(iterator->get());
	}

	// 使用不可
	// ポインタ渡しでも、関節参照でのオブジェクトのメンバにアクセスはできるが、ポインタ自体の変更はできない？
	/*template <typename T> bool TryGetComponent(T* component)
	{
		component = this->GetComponent<T>();

		if (component == nullptr)
		{
			return false;
		}

		Debug::Log(L"%d", component == nullptr);

		return true;
	}*/

	Transform* GetTransform();
	void SetActive(bool isActive);
	void Start();
	void Update();

	struct InstanceData
	{
		bool isVector = false;
		bool hasInstanceID = false;
		std::vector<std::string> memberVector;
	};

	void Deserialize(std::vector<std::string> instanceDataVector) override
	{
		std::vector<std::function<void(InstanceData*)>> functionVector = {
			// 以下マクロ定義
			[&](InstanceData* instanceData) -> void {
				DeserializeProperty(_componentVector, instanceData);
			}
		};

		bool isPacking = false;
		std::vector<InstanceData*> subInstanceDataVector;
		for (std::string instanceData : instanceDataVector)
		{
			std::smatch m;

			if (isPacking)
			{
				// リスト
				if (std::regex_match(instanceData, m, std::regex(R"(-\s(\w+):\s(\w+))")))
				{
					subInstanceDataVector.back()->isVector = true;

					// instanceIDをもつかどうか
					if (m[1].str() == "instanceID")
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


			std::regex re(R"(^(\w+):(\s*)(\w*))");
			if (std::regex_search(instanceData, m, re))
			{
				// クラス, 構造体, vector
				if (m[3].str() == "")
				{
					// 文字列vectorを確保
					// インデントの深さと"-"でチェック
					isPacking = true;
					subInstanceDataVector.push_back(new InstanceData());
				}
				// 値
				else
				{
					isPacking = false;

					// ここで代入
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


private:
	template <typename T>
	static void DeserializeProperty(T& variable, const InstanceData* instanceData)
	{
		if (instanceData->isVector)
		{
			for (auto member : instanceData->memberVector)
			{
				if (instanceData->hasInstanceID)
				{
					Object* object = SceneDataManager::GetInstanceID2PointerMap()[member];
					
					// 型チェック
					Component* component = dynamic_cast<Component*>(object);
					if (component != nullptr)
					{
						variable.push_back(std::shared_ptr<Component>(component));
						// AddComponentの処理を考えると、GameObjectだけ処理を特殊化しても良いかもしれない。
						// いや違う。componentのgameobjectもyamlで保存しておくから問題ない。
						// 自動でtransformがつくのが問題
						// かぶりなしのリストにすればよいのでは？
					}
					else
					{
						// GameObject
					}
				}
			}

			return;
		}

		// 再帰的にデシリアライズ
	}

	bool _isActive{true};
	Transform* _transform; // getcomponentで取得が良いかも
	std::vector<std::shared_ptr<Component>> _componentVector;
};
//REGISTER_TYPE(GameObject);
