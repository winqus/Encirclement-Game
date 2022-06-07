#pragma once

#include <vector>
#include "Bullet.h"

namespace enc {
	class Enemy : public GameObject {
	private:
		float _reloadSpeed;
		float _cooldownTime;
		float _bulletSpeed;
		float _bulletSpeedVariation;
		int _damage;
		float _shootingAngle;
		float _accuracy;
		GameObject* _target;
		BulletPool* _bulletPool;
	public:
		Enemy(float reloadSpeed, float bulletSpeed, float bulletSpeedVariation, int damage, float shootingAngle, float accuracy);
		void SetTarget(GameObject* target);
		GameObject* GetTarget() const;
		void SetBulletPool(BulletPool* bulletPool);
		void Shoot(float maxDistance);
		void Update(float deltaTime);
		//void ShootBullet(Bullet bullet);
	};
}