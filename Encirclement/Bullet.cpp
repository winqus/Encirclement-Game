#include "Bullet.h"
#include <iostream>

namespace enc {
	/* Bullet */
	Bullet::Bullet() {
		_health = 1;
		_speed = 50.0f;
		_damage = 1;
		_pCreator = NULL;
		_tag = "bullet";
		_startPosition = Vector2();
		_maxDistance = 0.0f;
	}
	Bullet::Bullet(const Bullet& bullet) {
		*this = bullet;
	}
	Bullet::Bullet(int health, float speed, int damage) {
		_health = clamp(health, 1, 1000);
		_speed = clamp(speed, 0.0f, 100000.0f);
		_damage = clamp(damage, 0, 100000);
		_pCreator = NULL;
		_tag = "bullet";
		_startPosition = Vector2();
		_maxDistance = 0.0f;
	}
	Bullet::Bullet(int health, float speed, int damage, float maxDistance) {
		*this = Bullet(health, speed, damage);
		this->SetMaxDistance(maxDistance);
	}
	void Bullet::SetStartPos(Vector2 position) {
		this->_startPosition = position;
	}
	void Bullet::Damage(unsigned value) {
		this->_health = clamp<unsigned>(_health - value, 0, 100);
	}
	GameObject* Bullet::Creator() {
		return this->_pCreator;
	}
	void Bullet::Creator(GameObject* pCreator) {
		this->_pCreator = pCreator;
	}

	void Bullet::SetMaxDistance(float distance)	{
		_maxDistance = clamp(distance, 0.0f, 100000000.0f);
	}

	void Bullet::Update(float deltaTime) {

		if (!this->isActive)
			return;
		
		/* DISTANCE */
		if (this->_maxDistance > 0.0f) { // If should check travelled distance
			if (this->transform.position().distance(this->_startPosition) > this->_maxDistance) {
				this->ReturnToPool();
				return;
			}	
		}

		this->transform.Translate(transform.direction() * deltaTime * _speed);

		if (this->_health <= 0) {
			this->ReturnToPool();
		}
		/* COLLISION HANDLING */
		for (auto collision : this->GetCollisions()) {
			GameObject* gameObject = collision->GetGameObject();
			auto tag = gameObject->GetTag();
			if (tag == "enemyBullet") {
				this->_health--;
			}
			else if (tag == "playerBullet") {
				this->_health--;
			}
			else if (tag == "player") {
				this->_health--;
				GameSoundMaster::AddSoundRequest("playerHit");
			}
			GameStats::IncreaseTagHitCount(tag);
			
		}

	}
	void Bullet::ReturnToPool() {
		this->isActive = false;
		this->_pCreator = nullptr;
		this->_startPosition = Vector2::Zero;
		if (this->collider != nullptr) {
			this->collider->SetTargetedTags({});
			this->collider->ResetCollisions();
			this->collider->Enabled(false);
		}
	}

	void Bullet::operator=(const Bullet& bullet) {
		this->_health = bullet._health;
		this->_speed = bullet._speed;
		this->_damage = bullet._damage;
		this->_pCreator = bullet._pCreator;
		//this->_targetTags = bullet._targetTags;
		this->_tag = bullet._tag;
		this->collider = bullet.collider;
		this->transform.scale(bullet.transform.scale());
		this->SetStartPos(bullet.transform.position());
		this->_maxDistance = bullet._maxDistance;
	}
	BulletPool::BulletPool() {
		_bullets = {};
	}
	BulletPool::~BulletPool() {
		Destroy();
		std::cout << "BulletPool destroyer called.\n";
	}
	Bullet* BulletPool::CreateBullet(Bullet bullet) {
		Bullet* ptr = new Bullet();
		*ptr = bullet;
		ptr->isActive = false;
		if(ptr->GetCollider() != nullptr)
			ptr->GetCollider()->SetGameObject(ptr);
		//ptr->GetCollider()->Enabled(false);
		_bullets.push_back(ptr);
		return ptr;
	}
	Bullet* BulletPool::FindInactiveBullet() {
		Bullet* pBullet = nullptr;
		for (Bullet* ptr : _bullets) {
			if (ptr != nullptr && ptr->isActive == false) {
				pBullet = ptr;
				break;
			}
		}
		return pBullet;
	}
	const std::vector<Bullet*> BulletPool::GetBullets() const{
		return _bullets;
	}
	void BulletPool::InstantiateBullet(Bullet bullet, Vector2 position, Vector2 direction) {
		Bullet* pBullet = nullptr;
		//pBullet = FindInactiveBullet(); //causes lag with many bullets
		if (pBullet == nullptr) {
			pBullet = CreateBullet(Bullet());
		}
		*pBullet = bullet;
		//pBullet->transform.scale(Vector2(0.7f, 0.7f));
		pBullet->transform.position(position);
		pBullet->transform.direction(direction);
		pBullet->SetStartPos(position);
		pBullet->isActive = true;
		pBullet->GetCollider()->SetGameObject(pBullet);
		pBullet->GetCollider()->Enabled(true);
		GameObject::MakeGameObjectGlobal(pBullet);
	}

	void BulletPool::Update(float deltaTime){
		for (Bullet* bullet : _bullets) {
			bullet->Update(deltaTime);
		}
	}

	void BulletPool::Destroy()	{
		for (Bullet* bullet : _bullets) {
			if (bullet == nullptr)
				return;
			bullet->Destroy();
			delete bullet;
			bullet = nullptr;
		}
	}



}