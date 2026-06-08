#pragma once

#include "Projectile.h"
#include<iostream>
using namespace std;

class StraightProjectile : public Projectile {
protected:
	float range; // can travel that much distanc
	float distance;
public:
	StraightProjectile(TextureManager* tex, int x, int y) : Projectile(tex, x, y) {
		gravityEffect = false;
	}

	virtual void update() override {
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

	virtual void velocityComponents(int angle) {
		velocity.y = -1 * sin(angle) * speed;
		velocity.x = cos(angle) * speed;
		if (direction == 2) {
			velocity.x *= -1;
		}
	}
};

class Bullet : public StraightProjectile {
private:

	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("bullet_first.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(23).setHeight(12);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("bullet_second.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(33).setHeight(14);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("bullet_third.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(8).setHeight(8);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("bullet_hit.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(83).setHeight(36);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("bullet_impact.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(83).setHeight(36);
		}

	}

public:
	Bullet(TextureManager* tex, int x, int y, int direction, int angle) : StraightProjectile(tex, x, y) {
		maxStates = 4;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 3; // 3 hp
		impactRadius = 0;
		this->angle = angle;
		scale.x = scale.y = 1.3;
		range = 15000;
		speed = 10;
		distance = 0;
		scale.y = 1;
		velocityComponents(angle);
		setAnimation(state);
	}

	void update() override {

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

class Rocket : public StraightProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("rocket_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(20).setPadding(5).setLoop(false);
			animation.setWidth(13).setHeight(16);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("rocket_move.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(24).setHeight(13);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("rocket_impact.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(16).setHeight(14);
		}

	}
public:
	Rocket(TextureManager* tex, int x, int y, int direction, int angle) : StraightProjectile(tex, x, y) {
		maxStates = 3;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 3; // 3 hp
		impactRadius = 0;
		this->angle = angle;
		scale.x = 1;
		range = 15000;
		speed = 15;
		distance = 0;
		scale.x= 2;
		scale.y = 2;
		velocityComponents(angle);
		setAnimation(state);
	}

};