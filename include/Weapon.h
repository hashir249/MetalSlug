#pragma once

#include <string>

#include "Entity.h"
#include "Projectile.h"
#include "BallisticProjectile.h"

using namespace std;
using namespace sf;

class Weapon : public Entity {
protected:
	const string name;
	const bool drawable;
	Clock timer;
	int coolDown;
	int angle;
	int ammo;
	void setAnimation(int state) override {}
public:
	Weapon(string n,TextureManager* tex,AudioManager*aud, int x, int y, bool draw = 1) : Entity(tex,aud, x, y), drawable(draw),name(n) {
		angle = 0;
	}

	string getName() const {
		return name;
	}

	bool isWeapon(string n) {
		return this->name == n;
	}

	virtual Entity* fire() = 0;
	virtual bool canFire() {
		return (timer.getElapsedTime().asMilliseconds() >= coolDown) && (ammo > 0);
	}

	// overrides
	void update() override {
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
	}
	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		if (this->hide == true || !drawable) return;

		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); 
		sprite.setPosition(position.x - scroll_x, position.y - scroll_y);
		window.draw(sprite);
	}

	void addAmmo(int a) {
		ammo += a;
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
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("shotgun_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(69).setHeight(19).setReversed(true);
		}
		else if (state == 1) { // down
			animation.setTexture(textureManager->getTexture("shotgun_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(65).setHeight(31).setReversed(true);
		}
		else if (state == 2) { // up
			animation.setTexture(textureManager->getTexture("shotgun_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(18).setHeight(60).setReversed(true);
		}
		else if (state == 3) { // side
			animation.setTexture(textureManager->getTexture("shotgun_side.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(8).setHeight(60).setReversed(true);
		}
	}
public:
	ShotGun(TextureManager* tex, AudioManager* aud, int x, int y,int direction, bool drawable = 1) : Weapon("ShotGun", tex, aud, x, y) {
		coolDown = 1000;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 4;
		state = 1;
		setAnimation(state);
		ammo = 20;
	}

	virtual Entity* fire() override;
};

class MachineGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("mg_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(60).setHeight(23).setReversed(true);
		}
		else if (state == 1) { // down
			animation.setTexture(textureManager->getTexture("mg_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(55).setHeight(31).setReversed(true);
		}
		else if (state == 2) { // up
			animation.setTexture(textureManager->getTexture("mg_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(9).setHeight(58).setReversed(true);
		}
		else if (state == 3) { // side
			animation.setTexture(textureManager->getTexture("mg_side.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(22).setHeight(57).setReversed(true);
		}
	}
public:
	MachineGun(TextureManager* tex, AudioManager* aud, int x, int y, int direction, bool drawable = 1) : Weapon("MachineGun", tex, aud, x, y) {
		coolDown = 100;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 4;
		state = 1;
		setAnimation(state);
		ammo = 100;
	}

	virtual Entity* fire() override;
};

class FlameGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("flamegun_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(58).setHeight(20).setReversed(true);
		}
		else if (state == 1) { // down
			animation.setTexture(textureManager->getTexture("flamegun_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(58).setHeight(24).setReversed(true);
		}
		else if (state == 2) { // up
			animation.setTexture(textureManager->getTexture("flamegun_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(10).setHeight(58).setReversed(true);
		}
		else if (state == 3) { // side
			animation.setTexture(textureManager->getTexture("flamegun_side.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(20).setHeight(58).setReversed(true);
		}
	}
public:
	FlameGun(TextureManager* tex,AudioManager* aud, int x, int y, int direction, bool drawable = 1) : Weapon("FlameGun", tex, aud, x, y) {
		coolDown = 100;
		scale.x = scale.y = 1;
		angle = 0;
		this->direction = direction;
		maxStates = 4;
		state = 1;
		setAnimation(state);
	}
	virtual Entity* fire() override;
};

class Bazooka : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("bazooka_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(42).setHeight(16).setReversed(true);
		}
		else if (state == 1) { // down
			animation.setTexture(textureManager->getTexture("bazooka_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(41).setHeight(19).setReversed(true);
		}
		else if (state == 2) { // up
			animation.setTexture(textureManager->getTexture("bazooka_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(10).setHeight(58).setReversed(true);
		}
		else if (state == 3) { // side
			animation.setTexture(textureManager->getTexture("bazooka_side.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(16).setHeight(41).setReversed(true);
		}
	}
public:
	Bazooka(TextureManager* tex, AudioManager* aud, int x, int y, int direction, bool drawable = 1) : Weapon("Bazooka", tex, aud, x, y) {
		coolDown = 1000;
		scale.x = scale.y = 1.6;
		angle = 0;
		this->direction = direction;
		maxStates = 4;
		state = 1;
		setAnimation(state);
		ammo = 10;
	}

	virtual Entity* fire() override;
};

class LaserGun : public Weapon {
	void setAnimation(int state) override {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("lasergun_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(69).setHeight(32).setReversed(true);
		}
		else if (state == 1) { // down
			animation.setTexture(textureManager->getTexture("lasergun_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(70).setHeight(35).setReversed(true);
		}
		else if (state == 2) { // up
			animation.setTexture(textureManager->getTexture("lasergun_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(69).setReversed(true);
		}
		else if (state == 3) { // side
			animation.setTexture(textureManager->getTexture("lasergun_side.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(18).setHeight(69).setReversed(true);
		}
	}
public:
	LaserGun(TextureManager* tex, AudioManager* aud, int x, int y, int direction, bool drawable = true) : Weapon("LaserGun", tex, aud, x, y) {
		coolDown = 1000;
		scale.x = scale.y = 0.7;
		angle = 0;
		this->direction = direction;
		maxStates = 4;
		state = 1;
		setAnimation(state);
	}
	virtual Entity* fire() override;
};

