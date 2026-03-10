#pragma once

#include "EngineFramework.h"
#include "Utility.h"
#include "Record.h"

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

	bool IsPrefab = false;
	Scene* scene = nullptr;
	std::string tag = "Untagged";

	NAME(GameObject)

	SERIALIZE3(Object,
		SERIALIZE_FIELD3(IsPrefab),
		SERIALIZE_FIELD3(tag),
		SERIALIZE_FIELD3(_componentVector),
		SERIALIZE_FIELD3(_isActive)
	)

	GameObject();

	// 関数テンプレートは型やコンパイル時に定まる値をパラメータ化する機能
	// 利用されるときに実体化するため、正しく分割コンパイルされない
	template <typename T> T* AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>();
		component.get()->gameObject = this;
		_componentVector.push_back(component);
		return component.get();
	}

	void AddComponent(Component* component);
	void RemoveComponent(const std::shared_ptr<Component>& component);

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

	Transform* GetTransform();
	const std::vector<std::shared_ptr<Component>>& GetComponentVector();
	void SetActive(bool isActive);
	bool ActiveSelf();
	void Start();
	void Restart();
	void Update();
	void EditorUpdate() override;
	

private:
	Record<bool> _isActive = true;
	std::vector<std::shared_ptr<Component>> _componentVector;
};
//REGISTER_TYPE(GameObject);
