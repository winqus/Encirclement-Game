#pragma once

#include "Transform.h"
#include "Collider.h"
#include <vector>
#include <string>
#include <iostream>

namespace enc {
	class Collider;
	class GameObject {
	protected:
		static unsigned int _gameObjectCount;
		unsigned int _id;
		std::string _tag;
		Collider* collider;
	public:
		Transform transform;
		bool isActive;
		GameObject();
		~GameObject();
		const GameObject* GetGameObject() const;
		const unsigned int GetId() const;
		static void MakeGameObjectGlobal(GameObject *gameObject);
		void AddCollider(float radius, Vector2 localPosition, bool enabled);
		void Destroy();
		Collider* GetCollider() const;
		void SetTag(std::string tagName);
		std::string GetTag() const;
		std::vector<const Collider*> GetCollisions() const;

	};
	std::vector<GameObject*> &GetGameObjects();
}

