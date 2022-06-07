#include "Player.h"

namespace enc {
	/* Player */
	Player::Player(int health, float reloadSpeed) {
		_health = clamp(health, 1, 1000000);
		_reloadSpeed = clamp(reloadSpeed, 0.1f, 10000.0f);
		autofire = false;
		_tag = "player";
		isActive = true;
		MakeGameObjectGlobal(this);
	}
	Player::~Player() {
		this->Destroy();
	}
	void Player::ShootBullet(BulletPool* bulletPool, Vector2 position, float maxDistance) {
		if (this->isActive == false)
			return;
		if (bulletPool == nullptr)
			return;
		if (_cooldownTime > 0.0f)
			return;

		Vector2 spawnPos = transform.position() + (transform.direction() * 2.0f);
		Bullet newBullet(3, 500.f, 1, maxDistance);
		newBullet.SetTag("playerBullet");
		newBullet.AddCollider(3.0f, Vector2::Zero, true);
		newBullet.GetCollider()->SetTargetedTags({ "enemyBullet" });
		bulletPool->InstantiateBullet(newBullet, spawnPos, transform.direction());

		GameSoundMaster::AddSoundRequest("playerShoot");

		_cooldownTime = _reloadSpeed;
	}
	void Player::Update(float deltaTime) {
		if (_cooldownTime > 0.0f)
			_cooldownTime -= deltaTime;
	}
	int Player::GetHealth() const {
		return _health;
	}
}