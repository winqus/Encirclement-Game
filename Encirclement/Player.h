#pragma once


#include <memory>
#include <queue>
#include <vector>
#include "Bullet.h"

namespace enc {
	class Player : public GameObject {
	public:
		bool autofire;
		Vector2 target;
	private:
		int _health;
		float _reloadSpeed;
		float _cooldownTime;
	public:
		Player(int health, float reloadSpeed);
		~Player();
		void ShootBullet(BulletPool* bulletPool, Vector2 position, float maxDistance);
		void Update(float deltaTime);
		int GetHealth() const;
	};
}