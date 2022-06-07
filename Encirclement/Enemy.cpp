#include "Enemy.h"

namespace enc {
	/* Enemy */
	Enemy::Enemy(float reloadSpeed, float bulletSpeed, float bulletSpeedVariation, int damage, float shootingAngle, float accuracy) {
		_reloadSpeed = clamp(reloadSpeed, 0.1f, 1000000.0f);
		_cooldownTime = _reloadSpeed;
		_bulletSpeed = bulletSpeed;
		_bulletSpeedVariation = bulletSpeedVariation;
		_damage = clamp(damage, 0, 10000);
		_shootingAngle = clamp(shootingAngle, 5.0f, 360.0f);
		_accuracy = clamp(accuracy, 0.0f, 1.0f);
		_target = NULL;
		_tag = "enemy";
		isActive = false;
		transform.scale(Vector2(0.5f, 0.5f));
	}
	void Enemy::SetTarget(GameObject* target) {
		_target = target;
	}
	GameObject* Enemy::GetTarget() const {
		return _target;
	}
	void Enemy::SetBulletPool(BulletPool* bulletPool) {
		_bulletPool = bulletPool;
	}
	void Enemy::Shoot(float maxDistance) {
		if (_bulletPool == nullptr)
			return;
		if (_cooldownTime > 0.0f)
			return;

		float newSpeed = _bulletSpeed;
		if (RandomNumber(0.0f, 1.0f, 4) >= 0.8f) {
			newSpeed += RandomNumber(0.0f, _bulletSpeedVariation * 2.0f) - _bulletSpeedVariation;
		}

		Bullet newBullet(1, newSpeed, _damage, maxDistance);
		newBullet.AddCollider(3.0f, Vector2::Zero, true);
		newBullet.GetCollider()->SetTargetedTags({ "player", "playerBullet" });
		newBullet.SetTag("enemyBullet");
		newBullet.transform.scale(Vector2(0.7f, 0.7f));
		

		if (RandomNumber(0.0f, 1.0f, 4) >= _accuracy) {
			float angleChange = RandomNumber(0.0f, _shootingAngle * 2.0f) - _shootingAngle;
			this->transform.Rotate(angleChange);
		}
		Vector2 spawnPos = this->transform.position() + (this->transform.direction() * 30.0f);
		_bulletPool->InstantiateBullet(newBullet, spawnPos, this->transform.direction());
		GameSoundMaster::AddSoundRequest("enemyShoot");

		this->transform.LookAt(this->_target->transform.position());
		_cooldownTime = _reloadSpeed;
	}
	void Enemy::Update(float deltaTime) {
		/* Target */
		if (this->_target == NULL)
			return;

		this->transform.LookAt(this->_target->transform.position());

		if (_cooldownTime > 0.0f)
			_cooldownTime -= deltaTime;

	}
}