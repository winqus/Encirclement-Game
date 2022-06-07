#include "GameObject.h"

namespace enc {
	/* GameObject */
	std::vector<GameObject*> gameObjects;
	unsigned int GameObject::_gameObjectCount = 0;
	GameObject::GameObject() {
		_id = _gameObjectCount;
		_gameObjectCount++;
		_tag = "";
		transform = Transform();
		collider = nullptr;
		isActive = false;
	}
	GameObject::~GameObject() {
		collider = nullptr;
	}
	const GameObject* GameObject::GetGameObject() const {
		return this;
	}
	const unsigned int GameObject::GetId() const {
		return _id;
	}
	std::vector<GameObject*> &GetGameObjects() {
		std::vector<GameObject*> newV;
		for (auto obj : gameObjects) {
			if (obj != nullptr)
				newV.push_back(obj);
		}
		gameObjects = newV;
		return gameObjects;
	}
	void GameObject::AddCollider(float radius, Vector2 localPosition, bool enabled) {
		this->collider = new Collider(radius, localPosition, this, enabled);
		this->collider->Enabled(enabled);
	}
	void GameObject::Destroy() {
		delete collider;
		collider = nullptr;
	}
	void GameObject::MakeGameObjectGlobal(GameObject *gameObject) {
		gameObjects.push_back(gameObject);
	}
	Collider* GameObject::GetCollider() const {
		return collider;
	}
	void GameObject::SetTag(std::string tagName) {
		_tag = tagName;
	}
	std::string GameObject::GetTag() const {
		return _tag;
	}
	std::vector<const Collider*> GameObject::GetCollisions() const
	{
		if (this->collider == nullptr)
			return {};
		std::vector<const Collider*> cols = {};
		for (auto col : collider->GetCollisions()) {
			if (col == nullptr || col->Enabled() == false)
				continue;
			auto gameObj = col->GetGameObject();
			if (gameObj == nullptr || gameObj->isActive == false)
				continue;

			cols.push_back(col);
		}
		return cols;
	}
}
