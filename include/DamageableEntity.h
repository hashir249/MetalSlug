#pragma once

#include "Entity.h"

class DamageableEntity : public Entity {
protected:
	int hp;
	bool invincible;
	int invincibleDuration;
	Clock damageTimer;
public:
	DamageableEntity(TextureManager* tex, AudioManager* aud,int x, int y) : Entity(tex,aud, x, y) {
		invincible = false;
		hp = 10;
	}
	virtual void takeDamage(int damage) = 0;
	virtual void takeHit() = 0;

	int getHp() const {
		return hp;
	}

	bool isDead() const {
		return hp <= 0;
	}
	bool isInvincible() const {
		return invincible;
	}
};