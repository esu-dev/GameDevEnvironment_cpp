#pragma once

#include "framework.h"
#include "Component.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	// 関数テンプレートは型やコンパイル時に定まる値をパラメータ化する機能
	// 利用されるときに実体化するため、正しく分割コンパイルされない
	template <typename T> T& AddComponent()
	{
		T* component = new T();
		m_componentVector.push_back(component);
		return *component;
	}

	template <typename T> T& GetComponent()
	{
		auto iterator = std::find_if(m_componentVector.begin(), m_componentVector.end(), [](Component* x) { return typeid(&x) == typeid(T); });
		int index = std::distance(m_componentVector.begin(), iterator);
		T* component = dynamic_cast<T*>(m_componentVector[0]); // dynamic_castは抽象クラスにしかダウンキャストととして利用できない
		return *component;
	}

	void Test();
	void Update();

private:
	std::vector<Component*> m_componentVector;
};