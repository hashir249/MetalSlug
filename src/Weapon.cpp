#include "StraightProjectile.h"
#include "Weapon.h"

Entity* ShotGun::fire()  {
		if (!canFire()) return nullptr;
		timer.restart();
		int x = hitbox.width + position.x;
		int y = position.y + hitbox.height / 2;
		if (direction == 2) x -= hitbox.width * 2;
		ammo = (ammo < 0) ? 0 : ammo;
		return new Bullet(textureManager,audioManager,x, y, direction, angle);
}

Entity* MachineGun::fire() {
		if (!canFire()) return nullptr;
		timer.restart();
		int x = hitbox.width + position.x;
		int y = position.y + hitbox.height / 2;
		if (direction == 2) x -= hitbox.width * 2;
		ammo--;
		ammo = (ammo < 0) ? 0 : ammo;
		return new Bullet(textureManager, audioManager, x, y, direction, angle);
}

Entity* FlameGun::fire()  {
		if (!canFire()) return nullptr;
		timer.restart();
		int x = hitbox.width + position.x;
		int y = position.y + hitbox.height / 2;
		if (direction == 2) x -= hitbox.width * 2;
		//ammo--;
		ammo = (ammo < 0) ? 0 : ammo;
		return new Flame(textureManager, audioManager, x, y, direction, angle);
}

Entity* Bazooka::fire() {
		if (!canFire()) return nullptr;
		timer.restart();
		int x = hitbox.width + position.x;
		int y = position.y + hitbox.height / 2;
		if (direction == 2) x -= hitbox.width * 2;
		ammo--;
		ammo = (ammo < 0) ? 0 : ammo;
		return new Rocket(textureManager, audioManager, x, y - 10, direction, angle);
	}

	Entity* LaserGun::fire() {
		if (!canFire()) return nullptr;
		timer.restart();
		int x = hitbox.width + position.x;
		int y = position.y + hitbox.height / 2;
		if (direction == 2) x -= hitbox.width * 2;
		ammo--;
		ammo = (ammo < 0) ? 0 : ammo;
		return new Bullet(textureManager, audioManager, x, y, direction, angle);
	}

