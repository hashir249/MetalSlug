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
	}

	

	Weapon* getWeapon(string n) {
		for (int i = 0; i < weaponCount; i++) {
			if (weapons[i]->isWeapon(n)) return weapons[i];
		}
		return nullptr;
	}

	int getWeaponCount() const {
		return weaponCount;
	}

	void setWeapon(string n) {
		for (int i = 0; i < weaponCount; i++) {
			cout << weapons[i]->getName() << endl;
			if (weapons[i]->isWeapon(n)) {
				currentWeapon = i;
				break;
			}
		}
	}

	void setLastWeapon() {
		currentWeapon = weaponCount - 1;
	}

	void addAmmo(int i, int a) {
		if(weaponCount > 0) weapons[i % weaponCount]->addAmmo(a);
	}

	Entity* fire() {
		Entity* result = nullptr;
		Weapon* w = getCurrentWeapon();
		if (w == nullptr) return nullptr;
		if (w->canFire()) {
			result = w->fire();
		}
		return result;
	}
	void setState(int state) {
		Weapon* w = getCurrentWeapon();
		if (w) w->setState(state);
	}

	void nextWeapon() {
		currentWeapon = (currentWeapon + 1) % weaponCount;
	}

	bool weaponExists(string name) {
		for (int i = 0; i < weaponCount; i++) {
			if (weapons[i]->isWeapon(name)) return true;
		}
		return false;
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

	int getWeaponState() {
		Weapon* w = getCurrentWeapon();
		if (w) return w->getState();
	}

	~Inventory() {
		if (weapons) {
			for (int i = 0; i < weaponCount; i++) {
				if (weapons[i]) delete weapons[i];
			}
		}
		
	}
};