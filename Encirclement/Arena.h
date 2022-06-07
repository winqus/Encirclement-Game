#pragma once

#include <vector>
#include <memory>
#include "Enemy.h"


namespace enc {
	class Arena : public GameObject {
	public:
		bool engage;
	private:
		float _radius;
		std::vector<Enemy*> _turrets;
		float _minGapBetweenTurrets;
		GameObject* _target;
		BulletPool* _bulletPool;
		int _rotationCount;
		float _rotationSpeed;
		float _timePassedSinceRotation;    
		float _currentAngle;
		int _startTurretCount;

		float _reloadSpeed;
		float _cooldownTime;


		float _defaultTurret_reloadSpeed;
		float _defaultTurret_bulletSpeed;
		float _defaultTurret_bulletSpeedVariation;
		int	  _defaultTurret_damage;
		float _defaultTurret_shootingAngle;
		float _defaultTurret_accuracy;

	public:
		Arena(float startRadius, int startTurretCount = 1, float reloadSpeed = 0.5f, float minGapBetweenTurrets = 150.0f, float rotationSpeed = 20.0f);
		~Arena();
		float GetRadius() const;
		void SetTarget(GameObject* target);
		void SetBulletPool(BulletPool* bulletPool);
		void SetTurretPreferences(float reloadSpeed, float bulletSpeed, float bulletSpeedVariation, int damage, float shootingAngle, float accuracy);
		GameObject* GetTarget() const;
		Enemy TurretBasedOnDefaultPreferences() const;
		void AddTurret(Enemy turret);
		void DestroyTurrets();
		void Update(float deltaTime);
		void Attack();
		const std::vector<Enemy*>& GetTurrets();
	};
}