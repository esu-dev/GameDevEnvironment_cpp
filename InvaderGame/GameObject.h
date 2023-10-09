#pragma once

#include "framework.h"

class Component;
class Transform;
class TextLabel;
class SpriteRenderer;

class GameObject
{
public:
	GameObject();

	Transform* transform;

	// 関数テンプレートは型やコンパイル時に定まる値をパラメータ化する機能
	// 利用されるときに実体化するため、正しく分割コンパイルされない
	template <typename T> T* AddComponent()
	{
		T* component = new T();
		m_componentVector.push_back(component);
		m_componentVector.back()->gameObject = this;
		return component;
	}

	template <typename T> T* GetComponent()
	{
		auto iterator = std::find_if(m_componentVector.begin(), m_componentVector.end(), [](Component* x) { return typeid(*x) == typeid(T); });
		if (iterator == m_componentVector.end())
		{
			return nullptr;
		}
		int index = std::distance(m_componentVector.begin(), iterator);
		T* component = dynamic_cast<T*>(m_componentVector[index]); // dynamic_castは抽象クラスにしかダウンキャストととして利用できない
		return component;
	}

	void Update();

private:
	std::vector<Component*> m_componentVector;
};