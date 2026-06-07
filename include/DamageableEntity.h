#pragma once

#include "Entity.h"

class DamageableEntity : public Entity {
protected:
	int hp;
	bool invincible;
	int invincibleDuration;
	Clock damageTimer;
public:
	DamageableEntity(TextureManager* tex, int x, int y) : Entity(tex, x, y) {
		invincible = false;
		hp = 100;
	}
	virtual void takeDamage(int damage) = 0;
	virtual void takeHit() = 0;


	bool isDead() const {
		return hp <= 0;
	}
	bool isInvincible() const {
		return invincible;
	}
};