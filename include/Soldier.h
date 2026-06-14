#pragma once
#include "Entity.h"
#include "DamageableEntity.h"
#include "Inventory.h"

class Soldier : public DamageableEntity {
protected:
	float jumpForce;
	Inventory* inventory; // as soldier does not own it
	sf::Vector2f weaponPlug;
public:
	Soldier(TextureManager* tex,AudioManager* aud, int x, int y) : DamageableEntity(tex,aud, x, y) {
		jumpForce = 15;
		inventory = new Inventory;
		product = nullptr; // till now there is no projectile in the start
		// will be created when fire is shot
	}

	void virtual attack() = 0;

	Entity* productEntity() override {
		Entity* temp = product;
		product = nullptr;
		return temp;
	}

	~Soldier() {
		if (product) delete product;
	}

	void interactWithPlayer(PlayerSoldier* p) override {

	}

	void interactWithProjectile(Projectile* p) override {

	}

	void interactWithVehicle(Vehicle* v) override {

	}
	void interactWithCollectible(Collectible* c) override {

	}
	void interactWithEnemy(Enemy* c) override {

	}
	void interactWithTerrain(Block* c) override {

	}
};