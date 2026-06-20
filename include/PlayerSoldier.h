#pragma once
#include "Soldier.h"
#include "Vehicle.h"
//#include "Collectible.h"
using namespace std;
//class Vehicle;

class PlayerSoldier : public Soldier {
protected:
	// adding time for mounting and unmountin
	Clock mountingTimer;
	// grenades
	int grenades;

	bool moving;
	int speed;
	int saturationStat;
	int legFactor; // for centering teh legs
	int gapFactor; // for reducing the gap between the legs and body
	AnimationManager animationLegs;
	sf::Sprite legs; // as for body sprite woudl be use 
	virtual void setAnimation(int state) {}
	virtual void updateWeaponPlug() {
		int state = inventory->getWeaponState();
		if (state == 0) {
			weaponPlug = sf::Vector2f(5,11);
		}
		else if (state == 1) {
			weaponPlug = sf::Vector2f(20, 11);
		}
		else if (state == 2) {
			weaponPlug = sf::Vector2f(20, 11);
		}
		else if (state == 3) {
			weaponPlug = sf::Vector2f(20, 11);
		}
	}

	Entity* throwGrenade(){
		if (grenades <= 0) return nullptr;
		grenades--;
		int x = position.x;
		int y = position.y;
		return new Grenade(textureManager,audioManager,x,y - 100,direction,75);
	}
public:
	PlayerSoldier(TextureManager* t, AudioManager* aud,int x, int y) : Soldier(t,aud, x, y) {
		speed = 11;
		moving = false;
		maxStates = 6;
		direction = 1;
		jumpForce = 25;
		saturationStat = 50;
		onGround = false;
		gravityEffect = true;
		grenades = 20;
		inventory->addWeapon(new Bazooka(textureManager, audioManager, position.x, position.y, direction));
		//inventory->addWeapon(new ShotGun(textureManager, audioManager, position.x, position.y, direction));
		//inventory->addWeapon(new FlameGun( textureManager, audioManager, position.x, position.y, direction));
		inventory->addWeapon(new MachineGun(textureManager, audioManager, position.x, position.y, direction));
		inventory->addAmmo(0, 10000);
		updateWeaponPlug();
		inventory->setWeapon("MachineGun");
	}
	void giveStat(int amount) {
		this->saturationStat += amount;
	}

	void interact(Entity* other) override {
		other->interactWithPlayer(this);
	}

	void takeDamage(int damage) override {

	}
	void attack() override {

	}
	void takeHit() override {

	}
	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		if (this->hide == true) return;
		inventory->render(window, scroll_x, scroll_y);
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		legs.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y);
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		legs.setPosition(position.x + (direction == 1 ? legFactor : -legFactor) - scroll_x + 0.5 * (animation.getWidth() * scale.x), (scale.y * animation.getHeight()) + position.y - scroll_y - gapFactor); // the y position woudl eb y posioint of bdoy plys
		window.draw(legs);
		window.draw(sprite);
	}
	void hitBoxUpdate() override {
		float actualWidth = animation.getWidth() * scale.x;
		float actualHeight = animation.getHeight() * scale.y;
		float offset = 0.5f * actualWidth;

		if (direction == 1) {
			hitbox.left = position.x + offset;
		}
		else {
			hitbox.left = position.x + offset - actualWidth;
		}
		hitbox.top = position.y;
		hitbox.width = actualWidth;
		hitbox.height = actualHeight + (animationLegs.getHeight() * scale.y) - gapFactor;
	}

	Inventory* getInventory() {
		return inventory;
	}

	void update() override {
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		if (velocity.x != 0.f && onGround) moving = true;
		animation.apply(sprite).cycle();
		animationLegs.apply(legs).cycle();
		hitBoxUpdate();
		updateWeaponPlug();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory->update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}

	void handleInput();

	// override for setting the enemy flag on teh projectiles
	virtual Entity* productEntity() override {
		Entity* product = this->product;
		if (product != nullptr) product->setEnemyFlag(false);
		this->product = nullptr;
		return product;
	}

	void interactWithVehicle(Vehicle* v) override;
};


class Tarma : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("tarma_idle_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(true);
			legFactor = 8;
			gapFactor = 21;
			inventory->setState(1);
		}
		else if (state == 1) { // running3
			animation.setTexture(textureManager->getTexture("tarma_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(35).setHeight(30).setReversed(true);
			gapFactor = 23;
			legFactor = 6;
			inventory->setState(1);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("tarma_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(true);
			gapFactor = 14;
			legFactor = 11;
			inventory->setState(3);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("tarma_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(89).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(24).setReversed(true);
			gapFactor = 10;
			legFactor = 11;
			inventory->setState(0);
		}
		else if (state == 4) { // melee
			animation.setTexture(textureManager->getTexture("tarma_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(49).setHeight(45).setReversed(true);
			legFactor = 20;
			gapFactor = 17;
			inventory->setState(3);
		}
		else if (state == 5) {// grenade lob
			animation.setTexture(textureManager->getTexture("tarma_grenade.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(39).setHeight(30).setReversed(true);
			legFactor = 16;
			gapFactor = 17;
			inventory->setState(3);
		}

		if (moving && onGround) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(125).setPadding(0).setLoop(true);
			animationLegs.setWidth(26).setHeight(20).setReversed(false);
		}
		else if (!onGround && moving) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}
		else if (!moving) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
	}

	void updateWeaponPlug() override {
		int state = inventory->getWeaponState();
		if (state == 0) {
			weaponPlug = sf::Vector2f(24 * scale.x, 12 * scale.y);
		}
		else if (state == 1 && moving) {
			weaponPlug = sf::Vector2f(18 * scale.x, 21 * scale.y);
		}
		else if (state == 1 && !moving) {
			weaponPlug = sf::Vector2f(16 * scale.x, 20 * scale.y);
		}
		else if (state == 2) {
			weaponPlug = sf::Vector2f(22, 25);
		}
		else if (state == 3 && onGround) {
			weaponPlug = sf::Vector2f(2 * scale.x, 34 * scale.y);
		}
		else if (state == 3 && !onGround) {
			weaponPlug = sf::Vector2f(2 * scale.x, 34 * scale.y);
		}

	}

public:
	Tarma(TextureManager* tex,AudioManager* aud, int x, int y) : PlayerSoldier(tex,aud, x, y) {
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		previousState = state;
		gapFactor = 21;
		hitBoxUpdate();
		moving = false;
	}
};


class Marco : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("marco_idle_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(30).setHeight(30).setReversed(true);
			gapFactor = 15;
			legFactor = 7;
			inventory->setState(1);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("marco_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(28).setReversed(true);
			gapFactor = 15;
			legFactor = -7;
			inventory->setState(1);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("marco_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(30).setReversed(true);
			gapFactor = 15;
			legFactor = 3;
			inventory->setState(3);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("marco_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(75).setPadding(0).setLoop(false);
			animation.setWidth(53).setHeight(24).setReversed(true);
			gapFactor = 11;
			legFactor = 3;
			inventory->setState(0);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("marco_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(46).setReversed(false);
			gapFactor = 16;
			legFactor = 30;
			inventory->setState(3);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("marco_grenade.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(36).setHeight(30).setReversed(false);
			gapFactor = 16;
			legFactor = 15;
			inventory->setState(3);
		}

		if (moving && onGround) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(125).setPadding(0).setLoop(true);
			animationLegs.setWidth(26).setHeight(20).setReversed(false);
		}
		else if (!onGround && moving) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}
		else if (!moving) {
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
	}

	void updateWeaponPlug() override {
		int state = inventory->getWeaponState();
		if (state == 0) {
			weaponPlug = sf::Vector2f(25.2*scale.x, 10.2*scale.y);
		}
		else if (state == 1 && moving) {
			weaponPlug = sf::Vector2f(21*scale.x,21*scale.y);
		}
		else if (state == 1 && !moving) {
			weaponPlug = sf::Vector2f(12 * scale.x, 21 * scale.y);
		}
		else if (state == 2) {
			weaponPlug = sf::Vector2f(22,25);
		}
		else if (state == 3 && onGround) {
			weaponPlug = sf::Vector2f(3.5*scale.x,34.5 * scale.y);
		}
		else if (state == 3 && !onGround) {
			weaponPlug = sf::Vector2f(11 * scale.x, 20 * scale.y);
		}
	}

public:
	Marco(TextureManager* tex,AudioManager*aud, int x, int y) : PlayerSoldier(tex,aud,x, y) {
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		previousState = state;
		gapFactor = 15;
		legFactor = 3;
	}
};


class Fiolina : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("fiolina_idle_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(29).setReversed(true);
			gapFactor = 39;
			legFactor = 5;
			inventory->setState(1);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("fiolina_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(31).setHeight(28).setReversed(true);
			gapFactor = 36;
			legFactor = 0;
			inventory->setState(1);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("fiolina_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 17;
			legFactor = 0;
			inventory->setState(3);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("fiolina_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(53).setHeight(22).setReversed(true);
			gapFactor = 24;
			legFactor = 0;
			inventory->setState(0);
		}
		else if (state == 4) { // melee
			animation.setTexture(textureManager->getTexture("fiolina_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(48).setHeight(41).setReversed(true);
			gapFactor = 40;
			legFactor = 17;
			inventory->setState(3);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("fiolina_grenade.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(35).setHeight(30).setReversed(false);
			gapFactor = 37;
			legFactor = 7;
			inventory->setState(3);
		}

		if (moving && onGround) {
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(129).setPadding(0).setLoop(true);
			animationLegs.setWidth(29).setHeight(26).setReversed(true);
		}
		else if (!onGround && moving) {
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(27).setHeight(22).setReversed(true);
		}
		else if (!moving) {
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

	void updateWeaponPlug() override {
		int state = inventory->getWeaponState();
		if (state == 0) {
			weaponPlug = sf::Vector2f(19.2 * scale.x, 9 * scale.y);
		}
		else if (state == 1 && moving) {
			weaponPlug = sf::Vector2f(17 * scale.x, 18 * scale.y);
		}
		else if (state == 1 && !moving) {
			weaponPlug = sf::Vector2f(17 * scale.x, 17 * scale.y);
		}
		else if (state == 2) {
			weaponPlug = sf::Vector2f(22, 25);
		}
		else if (state == 3 && onGround) {
			weaponPlug = sf::Vector2f(3 * scale.x, 28 * scale.y);
		}
		else if (state == 3 && !onGround) {
			weaponPlug = sf::Vector2f(4 * scale.x, 5 * scale.y);
		}
	}

public:
	Fiolina(TextureManager* tex, AudioManager* aud, int x, int y) : PlayerSoldier(tex,aud, x, y) {
		state = 0;
		setAnimation(state);
		moving = false;
		scale.x = scale.y = 2.5;
		gapFactor = 38;
		legFactor = 5;
		previousState = state;
		
	}
};


class Eri : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("eri_idle_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(29).setReversed(true);
			gapFactor = 37;
			legFactor = 12;
			inventory->setState(1);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("eri_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(28).setReversed(true);
			gapFactor = 30;
			legFactor = 7;
			inventory->setState(1);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("eri_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 20;
			legFactor = 0;
			inventory->setState(3);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("eri_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(22).setReversed(true);
			gapFactor = 18;
			legFactor = 0;
			inventory->setState(0);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("eri_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(48).setHeight(41).setReversed(true);
			gapFactor = 45;
			legFactor = 10;
			inventory->setState(3);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("eri_grenade.png"));
			animation.setCurrentFrame(0).setStartingFrame(1).setTotalFrames(5).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(38).setHeight(30).setReversed(false);
			gapFactor = 25;
			legFactor = 14;
			inventory->setState(3);
		}

		if (moving && onGround) {
			animationLegs.setTexture(textureManager->getTexture("eri_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(90).setPadding(0).setLoop(true);
			animationLegs.setWidth(24).setHeight(26).setReversed(true);
		}
		else if (!onGround && moving) {
			animationLegs.setTexture(textureManager->getTexture("eri_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(31).setHeight(22).setReversed(true);
		}
		else if (!moving) {
			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

	void updateWeaponPlug() override {
		int state = inventory->getWeaponState();
		if (state == 0) {
			weaponPlug = sf::Vector2f(24 * scale.x, 12 * scale.y);
		}
		else if (state == 1 && moving) {
			weaponPlug = sf::Vector2f(23 * scale.x, 21 * scale.y);
		}
		else if (state == 1 && !moving) {
			weaponPlug = sf::Vector2f(18 * scale.x, 17 * scale.y);
		}
		else if (state == 2) {
			weaponPlug = sf::Vector2f(22, 25);
		}
		else if (state == 3 && onGround) {
			weaponPlug = sf::Vector2f(3 * scale.x, 29 * scale.y);
		}
		else if (state == 3 && !onGround) {
			weaponPlug = sf::Vector2f(4 * scale.x, 10 * scale.y);
		}
	}
public:
	Eri(TextureManager* tex,AudioManager* aud, int x, int y) : PlayerSoldier(tex, aud,x, y) {
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		gapFactor = 32;
		legFactor = 10;
		previousState = state;
	}
};
