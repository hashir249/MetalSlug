#pragma once
#include "Weapon.h"

class Inventory {
	Weapon** weapons;
	int weaponCount;
	int weaponCap;
	int currentWeapon;

	Weapon* getCurrentWeapon() {
		return weapons[currentWeapon];
	}
public:
	Inventory() {
		weaponCount = 0;
		weaponCap = 2;
		weapons = new Weapon * [weaponCap] {};
		currentWeapon = 0;
	}

	bool fire() {
		if (getCurrentWeapon()->canFire()) {
			getCurrentWeapon()->fire();
		}
	}

	void nextWeapon() {
		currentWeapon = (currentWeapon + 1) % weaponCount;
	}

	void addWeapon(Weapon* w) {
		if (weaponCap == weaponCount) {
			Weapon** newWeapons = new Weapon * [weaponCap * 2];
			for (int i = 0; i < weaponCount; i++) {
				newWeapons[i] = weapons[i];
			}
			delete[] weapons;
			weapons = newWeapons;
			weaponCap *= 2;
		}
		weaponCount++;
		weapons[weaponCount - 1] = w;
	}

	void update(sf::Vector2f pos, int direction) {
		Weapon* w = getCurrentWeapon();
		if (w) {
			w->update();
			w->setPosition(pos);
			w->setDirection(direction);
		}
	}

	void render(RenderWindow& window, int x, int y) {
		Weapon* w = getCurrentWeapon();
		if (w) w->render(window, x, y);
	}

	~Inventory() {
		if (weapons) {
			for (int i = 0; i < weaponCount; i++) {
				if (weapons[i]) delete weapons[i];
			}
		}
		
	}
};