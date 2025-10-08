#pragma once

#include "framework.h"

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

	GameObject();

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


private:
	bool _isActive{true};
	Transform* _transform;
	std::vector<std::shared_ptr<Component>> _componentVector;
};