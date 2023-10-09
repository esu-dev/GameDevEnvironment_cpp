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

	// �֐��e���v���[�g�͌^��R���p�C�����ɒ�܂�l���p�����[�^������@�\
	// ���p�����Ƃ��Ɏ��̉����邽�߁A�����������R���p�C������Ȃ�
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
		T* component = dynamic_cast<T*>(m_componentVector[index]); // dynamic_cast�͒��ۃN���X�ɂ����_�E���L���X�g�ƂƂ��ė��p�ł��Ȃ�
		return component;
	}

	void Update();

private:
	std::vector<Component*> m_componentVector;
};