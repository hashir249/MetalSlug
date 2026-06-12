#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "StraightProjectile.h"
using namespace std;
using namespace sf;

class Weapon : public Entity {
protected:
	const bool drawable;
	Clock timer;
	int coolDown;
	int angle;
	void setAnimation(int state) override {}
public:
	Weapon(TextureManager* tex, int x, int y, bool draw = 1) : Entity(tex, x, y), drawable(draw) {

	}

	virtual Entity* fire() = 0;
	virtual bool canFire() = 0;

	// overrides
	void update() override {
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}
	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		if (this->hide == true) return;
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 1 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}
	void handleInput() override {}
	virtual void interact(Entity* other) override {}
	virtual void interactWithPlayer(PlayerSoldier*) override {}
	virtual void interactWithEnemy(Enemy*) override {}
	virtual void interactWithProjectile(Projectile*) override {}
	virtual void interactWithVehicle(Vehicle*) override {}
	virtual void interactWithCollectible(Collectible*) override {}
	virtual void interactWithTerrain(Block*) override {}
}; 

class ShotGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("shotgun_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(62).setHeight(23).setReversed(true);
		}
	}
public:
	ShotGun(TextureManager* tex, int x, int y,int direction, bool drawable = 1) : Weapon(tex,x ,y) {
		coolDown = 1000;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 1;
		state = 0;
		setAnimation(state);
	}

	virtual bool canFire() override {
		return timer.getElapsedTime().asMilliseconds() >= coolDown;
	}

	virtual Entity* fire() override {
		return new Bullet(textureManager, position.x, position.y, direction, angle);
	}
};

class MachineGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("mg_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(62).setHeight(23).setReversed(true);
		}
	}
public:
	MachineGun(TextureManager* tex, int x, int y, int direction, bool drawable = 1) : Weapon(tex, x, y) {
		coolDown = 1000;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 1;
		state = 0;
		setAnimation(state);
	}

	virtual bool canFire() override {
		return timer.getElapsedTime().asMilliseconds() >= coolDown;
	}

	virtual Entity* fire() override {
		return new Bullet(textureManager, position.x, position.y, direction, angle);
	}
};

class FlameGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("flamegun_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(62).setHeight(23).setReversed(true);
		}
	}
public:
	FlameGun(TextureManager* tex, int x, int y, int direction, bool drawable = 1) : Weapon(tex, x, y) {
		coolDown = 1000;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 1;
		state = 0;
		setAnimation(state);
	}

	virtual bool canFire() override {
		return timer.getElapsedTime().asMilliseconds() >= coolDown;
	}

	virtual Entity* fire() override {
		return new Bullet(textureManager, position.x, position.y, direction, angle);
	}
};