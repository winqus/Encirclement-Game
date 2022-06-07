#include "Collider.h"


namespace enc {
	/* Collider */
	Collider::Collider(float radius, Vector2 localPosition, GameObject* creator, bool enabled) {
		_radius = clamp(radius, 0.1f, 1000000.0f);
		_localPosition = localPosition;
		_gameObject = creator;
		_enabled = enabled;
	}
	Vector2 Collider::localPosition() const {
		return _localPosition;
	}
	void Collider::localPosition(Vector2 position) {
		_localPosition = position;
	}
	GameObject* Collider::GetGameObject() const {
		return _gameObject;
	}
	void Collider::SetGameObject(GameObject* gameObject) {
		_gameObject = gameObject;
	}
	bool Collider::Enabled() const {
		return _enabled;
	}
	void Collider::Enabled(bool state) {
		_enabled = state;
	}
	float Collider::GetRadius()	const {
		return _radius;
	}
	void Collider::SetRadius(float radius)	{
		_radius = clamp(radius, 1.0f, 100000000.0f);
	}

	bool Collider::IsTouchingCollider(const Collider* pCollider) {
		if (this->Enabled() == false)
			return false;
		if (pCollider == nullptr || pCollider->GetGameObject() == nullptr)
			return false;
		Vector2 pos1 = this->_gameObject->transform.position() + this->_localPosition;
		Vector2 pos2 = pCollider->GetGameObject()->transform.position() + pCollider->localPosition();
		float dist = pos1.distance(pos2);
		if (dist > this->GetRadius() + pCollider->GetRadius())
			return false;

		return true;
	}

	void Collider::CheckCollisions(){
		this->_collisions.clear();
		if (this->GetGameObject()->isActive == false)
			return;
		if (_targetTags.empty())
			return;
		if (this->Enabled() == false)
			return;
		auto& gameObjects = GetGameObjects();
		for (GameObject* gObj : gameObjects) {
			if (gObj == nullptr || gObj->isActive == false ||  gObj->GetTag() == "" || gObj->GetId() == this->GetGameObject()->GetId())
				continue;
			if (std::find(_targetTags.begin(), _targetTags.end(), gObj->GetTag()) == _targetTags.end())
				continue;
			const Collider* pCol = gObj->GetCollider();
			if (pCol == nullptr || pCol == this)
				continue;

			if (this->IsTouchingCollider(pCol)) {
				this->_collisions.push_back(pCol);
			}
			

		}

	}

	void Collider::SetTargetedTags(std::vector<std::string> tagNames) {
		this->_targetTags = tagNames;
	}

	std::vector<std::string> Collider::GetTargetedTags() const {
		return _targetTags;
	}

	void Collider::ResetCollisions() {
		_collisions.clear();
	}

	std::vector<const Collider*> Collider::GetCollisions() const {
		return _collisions;
	}

	void Collider::UpdateAllColliders() {
		auto& gameObjects = GetGameObjects();
		for (auto gameObject : gameObjects) {
			if (gameObject == nullptr || gameObject->GetCollider() == nullptr)
				continue;
			gameObject->GetCollider()->CheckCollisions();
			//auto collider = gameObject->GetCollider();
			//if (collider == nullptr)
			//	continue;
			//collider->ResetCollisions();
			//if (collider->GetTargetedTags().empty())
			//	continue;

			//for (auto gameObject2 : gameObjects) {

			//}
		}
	}
	
}