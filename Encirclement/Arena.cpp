#include "Arena.h"
#include <iostream>

namespace enc {
	/* Arena */
	Arena::Arena(float startRadius, int startTurretCount, float reloadSpeed, float minGapBetweenTurrets, float rotationSpeed) {
		engage = false;

		_radius = clamp(startRadius, 10.0f, 10000000.0f);
		_minGapBetweenTurrets = clamp(minGapBetweenTurrets, 0.0f, 1000000.0f);
		_rotationSpeed = clamp(rotationSpeed, 0.0f, 100000.0f);
		_target = NULL;
		_tag = "arena";
		isActive = true;
		_rotationCount = 0;
		_rotationSpeed = rotationSpeed;
		_timePassedSinceRotation = 0.0f;
		_currentAngle = 0.0f;
		_startTurretCount = startTurretCount;

		_reloadSpeed = reloadSpeed;
		_cooldownTime;

		SetTurretPreferences(1.0f, 50.0f, 20.0f, 1, 10.0f, 0.9f);
		MakeGameObjectGlobal(this);
	}
	Arena::~Arena() {
		DestroyTurrets();
		std::cout << "Arena destroyer called.\n";
	}
	float Arena::GetRadius() const {
		return _radius;
	}
	void Arena::SetTarget(GameObject* target) {
		_target = target;
		if (target == NULL)
			return;
		for (auto t : _turrets) {
			t->SetTarget(target);
		}
	}
	void Arena::SetBulletPool(BulletPool* bulletPool) {
		_bulletPool = bulletPool;
	}
	void Arena::SetTurretPreferences(float reloadSpeed, float bulletSpeed, float bulletSpeedVariation, int damage, float shootingAngle, float accuracy) {
		_defaultTurret_reloadSpeed = reloadSpeed;
		_defaultTurret_bulletSpeed = bulletSpeed;
		_defaultTurret_bulletSpeedVariation = bulletSpeedVariation;
		_defaultTurret_damage = damage;
		_defaultTurret_shootingAngle = shootingAngle;
		_defaultTurret_accuracy = accuracy;
	}
	GameObject* Arena::GetTarget() const {
		return _target;
	}
	Enemy Arena::TurretBasedOnDefaultPreferences() const {
		return Enemy(
			_defaultTurret_reloadSpeed,
			_defaultTurret_bulletSpeed,
			_defaultTurret_bulletSpeedVariation,
			_defaultTurret_damage,
			_defaultTurret_shootingAngle,
			_defaultTurret_accuracy
		); //(1.0f, 500.0f, 0.0f, 1, 30.0f, 1.0f);
	}
	void Arena::AddTurret(Enemy turret) {
		turret.SetTarget(_target);
		Enemy* pTurret = new Enemy(turret);
		pTurret->SetBulletPool(_bulletPool);

		pTurret->isActive = false;
		_turrets.push_back(pTurret);
		MakeGameObjectGlobal(pTurret);
	}
	void Arena::DestroyTurrets() {
		for (Enemy* pTurret : _turrets) {
			pTurret->Destroy();
			delete pTurret;
			pTurret = nullptr;
		}
	}
	void Arena::Update(float deltaTime) {
		while (_turrets.size() < _startTurretCount)
			AddTurret(Enemy(
				_defaultTurret_reloadSpeed,
				_defaultTurret_bulletSpeed,
				_defaultTurret_bulletSpeedVariation,
				_defaultTurret_damage,
				_defaultTurret_shootingAngle,
				_defaultTurret_accuracy
			)); //(1.0f, 500.0f, 0.0f, 1, 30.0f, 1.0f)

		

		int turretCount = (int)_turrets.size();
		float arenaLength = Circumference(_radius);
		float minGap = _minGapBetweenTurrets;
		if(minGap >= 10.f)
			for (int i = _rotationCount / 8; i != 0; i--)
				minGap *= 0.8;
		/* Turret positioning */
		float gap = arenaLength / turretCount;
		if (gap < minGap) {
			arenaLength += (minGap - gap) * turretCount;
			_radius = arenaLength / (2.f * (float)PI);
			gap = arenaLength / turretCount;
		}
		float alpha = 360.0f / turretCount; // the placement angle increment
		float currentAngle = 0.0f;
		for (int i = 0; i < turretCount; i++) {
			currentAngle = alpha * i;
			Vector2 placementPos = this->transform.position() + Vector2(_radius * sin(DegToRad(currentAngle)), _radius * cos(DegToRad(currentAngle))).rotate(transform.GlobalRotation());
			_turrets[i]->isActive = true;
			_turrets[i]->transform.position(placementPos);
			_turrets[i]->Update(deltaTime);
		}
		/*******************************/
		/* Rotation */
		if (transform.GlobalRotation() + (_rotationSpeed * deltaTime) > 360.0f) {
			_timePassedSinceRotation = 0.0f;
		}
		if (engage) {
			if (transform.GlobalRotation() + (_rotationSpeed * deltaTime) > 360.0f) {
				++_rotationCount;
				GameStats::arenaRotations = _rotationCount;
				Enemy newTurret(
					_defaultTurret_reloadSpeed,
					_defaultTurret_bulletSpeed,
					_defaultTurret_bulletSpeedVariation,
					_defaultTurret_damage,
					_defaultTurret_shootingAngle,
					_defaultTurret_accuracy
				); //(1.0f, 500.0f, 0.0f, 1, 30.0f, 1.0f)
				AddTurret(newTurret);
			
				if (_rotationCount % 8 == 0 && _defaultTurret_accuracy > 0.1f) {
					_defaultTurret_accuracy *= 0.5;
				}
			}

		}
			
		transform.Rotate(_rotationSpeed * deltaTime);
		/*******************************/

		_timePassedSinceRotation += deltaTime;
		if (engage && _rotationCount >= 0 && transform.GlobalRotation()  < 330.0f) {
			Attack();
		}

		if (_cooldownTime > 0.0f)
			_cooldownTime -= deltaTime;
	}
	void Arena::Attack() {
		if (_cooldownTime > 0.0f)
			return;
		for (auto t : _turrets) {
			if (RandomNumber(0.0f, 1.0f, 4) >= 0.90f) {
				t->Shoot(_radius*2);
				_cooldownTime = _reloadSpeed;
				//break;
			}
		}

	}
	const std::vector<Enemy*>& Arena::GetTurrets() {
		return this->_turrets;
	}
}