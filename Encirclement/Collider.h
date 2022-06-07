#pragma once

#include "GameObject.h"
#include <algorithm>
#include <vector>
#include <string>

namespace enc {
	class GameObject;

	class Collider {
	public: // static
		static void UpdateAllColliders();
	private:
		float _radius;
		Vector2 _localPosition;
		GameObject* _gameObject;
		std::vector<const Collider*> _collisions;
		std::vector<std::string> _targetTags;
		bool _enabled;
	public:
		Collider(float radius, Vector2 localPosition, GameObject* creator, bool enabled);
		Vector2 localPosition() const;
		void localPosition(Vector2 position);
		GameObject* GetGameObject() const;
		void SetGameObject(GameObject *gameObject);
		bool Enabled() const;
		void Enabled(bool state);
		float GetRadius() const;
		void SetRadius(float radius);
		bool IsTouchingCollider(const Collider* pCollider);
		void CheckCollisions();
		void SetTargetedTags(std::vector<std::string> tagNames);
		std::vector<std::string> GetTargetedTags() const;
		void ResetCollisions();
		std::vector<const Collider*> GetCollisions() const;
	};
}
