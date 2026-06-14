#pragma once
#include "Block.h"
#include "Entity.h"

class Level;

class CollisionManager {
	Level* level;
	float gravity = 1.5;
	bool solidAt(Entity* e, int x, int y) {
		sf::IntRect hb = e->getHitBox();

		bool left = level->isBlock(hb.left + x, hb.top + y);
		bool right = level->isBlock(hb.left + hb.width + x, hb.top + y);
		bool top = level->isBlock(hb.left + x, hb.top + hb.height + y);
		bool bottom = level->isBlock(hb.left + hb.width + x, hb.top + hb.height + y);

		return left || right || bottom || top;
	}

	void moveX(Entity* e, float amount) {

		int step = (amount > 0) ? 1 : -1;

		while (abs(amount) >= 1) {

			if (!solidAt(e, step, 0)) {
				e->setPosition(e->getPosition().x + step, e->getPosition().y);
				e->hitBoxUpdate();
				amount -= step;
			}
			else {
				e->updateCollision(true);
				e->setVelocityX(0);
				return;
			}
		}
	}

	void moveY(Entity* e, float amount) {

		int step = (amount > 0) ? 1 : -1;

		while (abs(amount) >= 1) {
			if (!solidAt(e, 0, step)) {
				e->setPosition(e->getPosition().x, e->getPosition().y + step);
				e->hitBoxUpdate();
				amount -= step;
			}
			else {
				if (step > 0) {
					e->setOnGround(true);
				}
				e->updateCollision(true);
				e->setVelocityY(0);
				return;
			}
		}
	}


	void applyGravity(Entity* e) {
		if (e->isGravityEffect()) {
			e->setVelocityY(e->getVelocityY() + gravity);
		}
	}

public:
	CollisionManager() {
		level = nullptr;
	}

	CollisionManager& setLevel(Level* lvl) {
		level = lvl;
		return *this;
	}

	void depenetrate(Entity* e)
	{
		const int MAX_PUSH = 100;

		int tries = 0;

		while (solidAt(e, 0, 0) && tries < MAX_PUSH)
		{
			float vx = e->getVelocityX();
			float vy = e->getVelocityY();

			// Prefer resolving along dominant movement axis
			if (abs(vx) > abs(vy))
			{
				if (vx > 0)
					e->setPosition(e->getPosition().x - 1, e->getPosition().y);
				else if (vx < 0)
					e->setPosition(e->getPosition().x + 1, e->getPosition().y);
				else
					e->setPosition(e->getPosition().x, e->getPosition().y - 1);
			}
			else
			{
				if (vy > 0)
					e->setPosition(e->getPosition().x, e->getPosition().y - 1);
				else if (vy < 0)
					e->setPosition(e->getPosition().x, e->getPosition().y + 1);
				else
					e->setPosition(e->getPosition().x, e->getPosition().y - 1);
			}

			e->hitBoxUpdate();
			tries++;
		}
	}

	void resolve(Entity* e) {
		if (!level) return;

		int safety = 100;

		while (solidAt(e, 0, 0) && safety--)
		{
			e->setPosition(e->getPosition().x,
				e->getPosition().y - 1);
			e->hitBoxUpdate();
		}

		e->setOnGround(false);
		applyGravity(e);
		moveX(e, e->getVelocityX());
		moveY(e, e->getVelocityY());

		e->hitBoxUpdate();
	}
};