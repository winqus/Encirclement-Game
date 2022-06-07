#pragma once

#include "GameObject.h"
#include "GameStats.h"
#include "GameSoundMaster.h"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <queue>

namespace enc {
	class BulletPool;

	class Bullet : public GameObject {
	public:
		static BulletPool* _bulletPool;
	protected:
		int _health;
		float _speed;
		int _damage;
		GameObject* _pCreator;

		Vector2 _startPosition;
		float _maxDistance;
	public:
		Bullet();
		Bullet(const Bullet& bullet);
		Bullet(int health, float speed, int damage);
		Bullet(int health, float speed, int damage, float maxDistance);
		void Damage(unsigned value);
		void Creator(GameObject* pCreator);
		void SetStartPos(Vector2 position);
		void SetMaxDistance(float distance);
		GameObject* Creator();

		void Update(float deltaTime);
		void ReturnToPool();
		void operator= (const Bullet& bulletToCopy);
	};

	class BulletPool {
	private:
		std::vector<Bullet*> _bullets;
		Bullet* FindInactiveBullet();
		Bullet* CreateBullet(Bullet bullet);
		
	public:
		BulletPool();
		~BulletPool();
		const std::vector<Bullet*> GetBullets() const;
		void InstantiateBullet(Bullet bullet, Vector2 position, Vector2 direction);
		void Update(float deltaTime);
		void Destroy();

	};
}