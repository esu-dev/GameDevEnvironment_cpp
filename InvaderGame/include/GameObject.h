#pragma once

#include "framework.h"

#include "Object.h"
#include "Scene.h"

class Component;
class Transform;
class TextLabel;
class SpriteRenderer;
class Animator;

using namespace SceneManagement;

class GameObject : public Object
{
public:
	GameObject();

	Scene* scene;

	// 関数テンプレートは型やコンパイル時に定まる値をパラメータ化する機能
	// 利用されるときに実体化するため、正しく分割コンパイルされない
	template <typename T> T* AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>();
		m_componentVector.push_back(component);
		m_componentVector.back()->gameObject = this;
		return component.get();
	}

	template <typename T> T* GetComponent()
	{
		auto iterator = std::find_if(m_componentVector.begin(), m_componentVector.end(), [](std::shared_ptr<Component> x)
			{ 
				return typeid(*x) == typeid(T) || dynamic_cast<T*>(x.get());
			});
		if (iterator == m_componentVector.end())
		{
			return nullptr;
		}
		//__int64 index = std::distance(m_componentVector.begin(), iterator);
		//T* component = dynamic_cast<T*>(m_componentVector[index]); // dynamic_castは抽象クラスにしかダウンキャストととして利用できない
		//return component;

		return dynamic_cast<T*>(iterator->get());
	}

	// ポインタ渡しでも、関節参照でのオブジェクトのメンバにアクセスはできるが、ポインタ自体の変更はできない？
	template <typename T> bool TryGetComponent(T* component)
	{
		component = this->GetComponent<T>();

		if (component == nullptr)
		{
			return false;
		}

		Debug::Log(L"%d", component == nullptr);

		return true;
	}

	Transform* GetTransform();
	void SetActive(bool isActive);
	void Start();
	void Update();


private:
	bool _isActive{true};
	Transform* _transform;
	std::vector<std::shared_ptr<Component>> m_componentVector;
};