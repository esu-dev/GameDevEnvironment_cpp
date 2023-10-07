#pragma once

#include "framework.h"
#include "Component.h"
#include "TextLabel.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	// �֐��e���v���[�g�͌^��R���p�C�����ɒ�܂�l���p�����[�^������@�\
	// ���p�����Ƃ��Ɏ��̉����邽�߁A�����������R���p�C������Ȃ�
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
		T* component = dynamic_cast<T*>(m_componentVector[0]); // dynamic_cast�͒��ۃN���X�ɂ����_�E���L���X�g�ƂƂ��ė��p�ł��Ȃ�
		return *component;
	}

	void Test();
	void Update();

private:
	std::vector<Component*> m_componentVector;
};