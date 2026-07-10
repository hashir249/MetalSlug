#pragma once

#include "Projectile.h"
#include "Soldier.h"
#include "Entity.h"
#include<iostream>
using namespace std;


class StraightProjectile : public Projectile {
protected:
	float range; // can travel that much distanc
	float distance;
public:
	StraightProjectile(TextureManager* tex,AudioManager* aud, int x, int y) : Projectile(tex,aud, x, y) {
		gravityEffect = false;
		active = true;
		distance = 0;
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
		animation.apply(sprite).cycle();
	}

	virtual void velocityComponents(int angle) {
		velocity.y = -1 * sin(angle) * speed;
		velocity.x = cos(angle) * speed;
		if (direction == 2) {
			velocity.x *= -1;
		}
	}

	// overrides
	virtual void interact(Entity* other) override {
		other->interactWithProjectile(this);
	}
	virtual void interactWithPlayer(PlayerSoldier*) override {}
	virtual void interactWithEnemy(Enemy*) override {}
	virtual void interactWithProjectile(Projectile*) override {}
	virtual void interactWithVehicle(Vehicle*) override {}
	virtual void interactWithCollectible(Collectible*) override {}
	virtual void interactWithTerrain(Block*) override {}
};

class Bullet : public StraightProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("bullet_first.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(50).setPadding(0).setLoop(false);
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
			animation.setTexture(textureManager->getTexture("bullet_impact.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(29).setHeight(30);
			scale = sf::Vector2f(2, 2);
		}

	}

public:
	Bullet(TextureManager* tex, AudioManager* aud, int x, int y, int direction, int angle) : StraightProjectile(tex,aud, x, y) {
		maxStates = 4;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 3; // 3 hp
		impactRadius = 0;
		bullet = true;
		this->angle = angle;
		scale.x = scale.y = 1.3;
		range = 1500;
		speed = 30;
		velocityComponents(angle);
		setAnimation(state);
	}

	void update() override {

		if (distance >= range) {
			active = false;
			return;
		}

		if (collided == true && state != 3) {
			state = 3;
			velocity = sf::Vector2f(0, 0);
		}
		if (animation.getDone()) {
			state++;
			//animation.setDone(false);
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
		animation.apply(sprite).cycle();

		distance += velocity.x;
		distance += velocity.y;
	}

	virtual void interactWithPlayer(PlayerSoldier*) override;
	virtual void interactWithEnemy(Enemy*) override;
};

class Rocket : public StraightProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("rocket_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(4).setDelay(20).setPadding(5).setLoop(false);
			animation.setWidth(22).setHeight(16);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("rocket_move.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(24).setHeight(12);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("bazooka_impact.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(27).setHeight(29);
			scale = sf::Vector2f(3,4);
		}

	}
public:
	Rocket(TextureManager* tex,AudioManager* aud, int x, int y, int direction, int angle) : StraightProjectile(tex,aud, x, y) {
		maxStates = 3;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 10; // 3 hp
		impactRadius = 0;
		this->angle = angle;
		range = 1000;
		speed = 20;
		distance = 0;
		scale.x = scale.y = 1.75;
		velocityComponents(angle);
		setAnimation(state);
	}

	void update() override {
		if (distance >= range) {
			active = false;
			return;
		}
		if (collided == true) {
			state = 2;
			velocity.x = 0;
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
		animation.apply(sprite).cycle();

		distance += velocity.x;
		distance += velocity.y;
	}
};

class Flame : public StraightProjectile {
	// for flame we will track the player movement to move the flame
	sf::Vector2f previousPosition;
	sf::Vector2f currentPosition;
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("flame_start.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(55).setHeight(25);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("flame_loop.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(70).setPadding(0).setLoop(true);
			animation.setWidth(65).setHeight(33);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("flame_stop.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(55).setHeight(33);
		}
	}

public:
	Flame(TextureManager* tex, AudioManager* aud, int x, int y, int direction, int angle) : StraightProjectile(tex, aud, x, y) {
		maxStates = 3;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 10; // 3 hp
		impactRadius = 0;
		this->angle = angle;
		range = 10000;
		speed = 0;
		distance = 0;
		scale = sf::Vector2f(3.2, 2);
		velocityComponents(angle);
		setAnimation(state);
		position.x -= 78;
	}

	void update() override {
		//cout << "Before" << endl;
		//cout << position.x << endl;
		//position.x += currentPosition.x - previousPosition.x;
		//cout << "After" << endl;
		//cout << position.x << endl;
		if (distance >= range) {
			active = false;
			return;
		}
		if (collided == true) {
			state = 2;
			velocity.x = 0;
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
		animation.apply(sprite).cycle();

		distance += velocity.x;
		distance += velocity.y;
	}

	virtual void setTarget(sf::Vector2f pos) override {
		previousPosition = currentPosition;
		currentPosition = pos;
	}
};

class TankRocket : public StraightProjectile {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("rocket_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(4).setDelay(20).setPadding(5).setLoop(false);
			animation.setWidth(22).setHeight(16);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("tank_rocket.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(50).setHeight(17);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("bazooka_impact.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(27).setHeight(29);
			scale = sf::Vector2f(3, 4);
		}

	}
public:
	TankRocket(TextureManager* tex, AudioManager* aud, int x, int y, int direction, int angle) : StraightProjectile(tex, aud, x, y) {
		maxStates = 3;
		state = 0;
		previousState = state;
		this->direction = direction;
		damage = 10; // 3 hp
		impactRadius = 0;
		this->angle = angle;
		range = 1000;
		speed = 20;
		distance = 0;
		scale.x = scale.y = 1.75;
		velocityComponents(angle);
		setAnimation(state);
	}

	void update() override {
		if (distance >= range) {
			active = false;
			return;
		}
		if (collided == true) {
			state = 2;
			velocity.x = 0;
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
		animation.apply(sprite).cycle();

		distance += velocity.x;
		distance += velocity.y;
	}
};