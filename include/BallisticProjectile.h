#pragma once

#include "Projectile.h"
#include "SFML/Graphics.hpp"
using namespace sf;

class BallisticProjectile : public Projectile {
protected:
	float gravity;
	float range;
	float distance;
	float rotationAngle;

public:

	BallisticProjectile(TextureManager* tex,AudioManager* aud, int x, int y) : Projectile(tex,aud, x, y) {
		gravity = 0.35f;
		range = 2000;
		distance = 0;
		speed = 8;
		velocity.x = 0;
		velocity.y = 0;
		rotationAngle = 0;
		active = true;
		gravityEffect = true;
		collided = false;
	}
	// initializes projectile launch vector
	virtual void velocityComponents(int angle) {
		velocity.x = cos(angle) * speed;
		velocity.y = -sin(angle) * speed;
		if (direction == 2) {
			velocity.x *= -1;
		}
	}

	void interactWithPlayer(PlayerSoldier* p) override {}
	void interactWithProjectile(Projectile* p) override {}
	void interactWithVehicle(Vehicle* v) override {}
	void interactWithCollectible(Collectible* c) override {}
	void interactWithEnemy(Enemy* c) override {}
	void interactWithTerrain(Block* c) override {}

};


class Grenade : public BallisticProjectile {

private:

	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("grenade_topple.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(16).setHeight(19);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("grenade_roll.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setDelay(100).setPadding(0).setLoop(true);
			animation.setWidth(23).setHeight(17).setReversed(false);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_first.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(48).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_second.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(150).setPadding(0).setLoop(false);
			animation.setWidth(62).setHeight(146).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_third.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(66).setHeight(153).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_fourth.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(70).setHeight(122).setReversed(false);
			scale.x= scale.y = 1;
		}
	}

public:

	Grenade(TextureManager* tex, AudioManager* aud, int x, int y, int direction, int angle) : BallisticProjectile(tex,aud,x, y) {
		this->direction = direction;
		damage = 25;
		impactRadius = 150;
		speed = 10;
		//gravity = 0.1;
		range = 3000;
		scale.x= 3;
		scale.y = 3;
		gravityEffect = true; // should be false, just for testing purposes
		state = 0;
		previousState = state;
		maxStates = 6; // last once is collision
		collided = false;
		velocityComponents(angle);
		setAnimation(state);
	}

	virtual void update() override {
		if (collided) {
			velocity.x = 0;
			velocity.y = 0;
			state++;
		}
		distance += speed;
		hitBoxUpdate();
		if (distance >= range) {
			active = false;
			return;
		}

		if (animation.getDone()) {
			state++;
			animation.setDone(false);
			if (state == maxStates) {
				active = false;
				return;
			}
			setAnimation(state);
			previousState = state;
		}

		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}

		animation.apply(sprite);
		animation.cycle();

		hitBoxUpdate();
	}
};

class FireBomb : public BallisticProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("firebomb_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(22).setHeight(32);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("firebomb_roll.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(39).setHeight(22).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("firebomb_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(1000).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(false);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_first.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(48).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_second.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(62).setHeight(146).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_third.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(68).setHeight(153).setReversed(false);
			scale.x= scale.y = 1;
		}
		else if (state == 6) {
			animation.setTexture(textureManager->getTexture("grenade_explosion_fourth.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(69).setHeight(122).setReversed(false);
			scale.x= scale.y = 1;
		}
	}

public:
	FireBomb(TextureManager* tex,AudioManager* aud, int x, int y, int direction, int angle) : BallisticProjectile(tex,aud, x, y) {
		this->direction = direction;
		damage = 25;
		impactRadius = 150;
		speed = 10;
		gravity = 0.1;
		range = 3000;
		scale.x= 2;
		scale.y = 2;
		state = 1;
		previousState = state;
		maxStates = 7;
		collided = false;
		velocityComponents(angle);
		setAnimation(state);

	}

	virtual void update() override {
		distance += speed;
		hitBoxUpdate();
		if (collided) {
			velocity.x = 0;
			velocity.y = 0;
			state++;
		}
		if (distance >= range) {
			active = false;
			return;
		}

		if (animation.getDone()) {
			state++;
			animation.setDone(false);
			if (state == maxStates) {
				active = false;
				return;
			}
			setAnimation(state);
			previousState = state;
		}

		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();

		hitBoxUpdate();
	}
};

// Missile
class Missile : public BallisticProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("long_range_missile_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(42).setHeight(34);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("long_range_missile_roll.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(50).setHeight(26).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("long_range_missile_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(1000).setPadding(0).setLoop(true);
			animation.setWidth(45).setHeight(33).setReversed(false);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("big_explosion_first.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(87).setHeight(104).setReversed(false);
			scale.x= scale.y = 2;
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("big_explosion_second.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(104).setHeight(117).setReversed(false);
			scale.x= scale.y = 2;
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("big_explosion_third.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(110).setHeight(120).setReversed(false);
			scale.x= scale.y = 2;
		}
		else if (state == 6) {
			animation.setTexture(textureManager->getTexture("big_explosion_fourth.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(106).setHeight(109).setReversed(false);
			scale.x= scale.y = 2;
		}
	}

public:
	Missile(TextureManager* tex,AudioManager* aud, int x, int y, int direction, int angle) : BallisticProjectile(tex,aud, x, y) {
		this->direction = direction;
		damage = 25;
		impactRadius = 150;
		speed = 10;
		gravity = 0.1;
		range = 3000;
		scale.x= 2;
		scale.y = 2;
		state = 0;
		previousState = state;
		maxStates = 3;
		collided = false;
		velocityComponents(angle);
		setAnimation(state);
	}

	virtual void update() override {
		distance += speed;
		hitBoxUpdate();
		if (collided) {
			velocity.x = 0;
			velocity.y = 0;
			state++;
		}
		if (distance >= range) {
			active = false;
			return;
		}

		if (animation.getDone()) {
			state++;
			animation.setDone(false);
			if (state == maxStates) {
				active = false;
				return;
			}
			setAnimation(state);
			previousState = state;
		}

		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();
	}
};