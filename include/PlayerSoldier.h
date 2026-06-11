#pragma once
#include "Soldier.h"
using namespace std;
class Vehicle;

class PlayerSoldier : public Soldier {
protected:
	int speed;
	float saturationStat;
	int legFactor; // for centering teh legs
	int gapFactor; // for reducing the gap between the legs and body
	AnimationManager animationLegs;
	sf::Sprite legs; // as for body sprite woudl be use 
	virtual void setAnimation(int state) {}
public:
	PlayerSoldier(TextureManager* t, int x, int y) : Soldier(t, x, y) {
		speed = 7.0f;
		direction = 1;
		scale.x = scale.y = 2;
		gapFactor = legFactor = 0;
		jumpForce = 20;
		saturationStat = 50;
		onGround = false;
		gravityEffect = true;
	}

	virtual void giveStat(int amount) override {
		this->saturationStat += amount;
	}

	void interact(Entity* other) override {
		other->interactWithPlayer(this);
	}

	bool inVehicle() const {
		return false;
	}

	void enterVehicle(Vehicle* v) {
		
	}

	void exitVehicle() {
		
	}

	void takeDamage(int damage) override {

	}

	void addSaturation(int amount) {
		this->saturationStat += amount;
	}
	void takeHit() override {

	}
	// overriden functions
	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		if (this->hide == true) return;
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
	void update() override {
		handleInput();


		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animationLegs.apply(legs);
		animation.cycle();
		animationLegs.cycle();
		hitBoxUpdate();
	}

	void handleInput() override {
		velocity.x = 0;
	
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			velocity.x = speed;
			direction = 1;
			if (onGround) state = 1;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left)) {
			velocity.x = -speed;
			direction = 2;
			if (onGround) state = 1;
		}
		else if (onGround) {
			if (state == 3 && !animation.getDone()) state = 3;
			if (state == 4 && !animation.getDone()) state = 4;
			else {
				state = 0;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) && onGround) {
			velocity.y = -jumpForce;
			onGround = false;
			state = 2;
		}
		if (state != 4) {
			if (Keyboard::isKeyPressed(Keyboard::M)) {
				state = 4;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			state = 3;
		}
		
		
	}
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
			//animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			//animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			//animationLegs.setWidth(21).setHeight(16).setReversed(true);
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("tarma_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(35).setHeight(30).setReversed(true);
			gapFactor = 23;
			legFactor = 6;
			//animationLegs.setTexture(textureManager->getTexture("tarma_legs_run.png"));
			//animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(100).setPadding(0).setLoop(true);
			//animationLegs.setWidth(37).setHeight(20).setReversed(true);
			animationLegs.setTexture(textureManager->getTexture("marco_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(125).setPadding(0).setLoop(true);
			animationLegs.setWidth(26).setHeight(20).setReversed(false);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("tarma_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(true);
			gapFactor = 14;
			legFactor = 11;
			//animationLegs.setTexture(textureManager->getTexture("tarma_legs_jump.png"));
			//animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			//animationLegs.setWidth(33).setHeight(22).setReversed(true);
			animationLegs.setTexture(textureManager->getTexture("marco_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("tarma_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(89).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(24).setReversed(true);
			gapFactor = 10;
			legFactor = 11;
			//animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			//animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			//animationLegs.setWidth(21).setHeight(16).setReversed(true);
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("tarma_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(49).setHeight(45).setReversed(true);
			legFactor = 20;
			gapFactor = 17;
			//animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			//animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(200).setPadding(0).setLoop(false);
			//animationLegs.setWidth(21).setHeight(16).setReversed(true);
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
	}

public:
	Tarma(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		previousState = state;
		gapFactor = 21;
		hitBoxUpdate();
	}

	void attack() override {
		//takeShot(position.x + (direction == 1 ? 30 : -30), position.y - 20);
	}

	void takeDamage(int damage) override {

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
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("marco_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(28).setReversed(true);
			gapFactor = 15;
			legFactor = -7;
			animationLegs.setTexture(textureManager->getTexture("marco_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(100).setPadding(0).setLoop(true);
			animationLegs.setWidth(26).setHeight(20).setReversed(false);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("marco_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(30).setReversed(true);
			gapFactor = 15;
			legFactor = 3;
			animationLegs.setTexture(textureManager->getTexture("marco_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("marco_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(75).setPadding(0).setLoop(false);
			animation.setWidth(53).setHeight(24).setReversed(true);
			gapFactor = 11;
			legFactor = 3;
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("marco_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(46).setReversed(false);
			gapFactor = 16;
			legFactor = 30;
			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
	}

public:
	Marco(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 4;
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		previousState = state;
		gapFactor = 15;
		legFactor = 3;
	}
	void attack() override {
		//takeShot(position.x + (direction == 1 ? 30 : -30), position.y - 20);
	}
	void takeDamage(int damage) override {

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
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("fiolina_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(31).setHeight(28).setReversed(true);
			gapFactor = 36;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(129).setPadding(0).setLoop(true);
			animationLegs.setWidth(29).setHeight(26).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("fiolina_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 17;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(27).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("fiolina_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(53).setHeight(22).setReversed(true);
			gapFactor = 24;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state = 4) {
			animation.setTexture(textureManager->getTexture("fiolina_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(48).setHeight(41).setReversed(true);
			gapFactor = 40;
			legFactor = 17;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

public:
	Fiolina(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		gapFactor = 38;
		legFactor = 5;
		previousState = state;
	}

	void attack() override {
		
	}
	void takeDamage(int damage) override {

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
			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("eri_run_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(28).setReversed(true);
			gapFactor = 30;
			legFactor = 7;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(90).setPadding(0).setLoop(true);
			animationLegs.setWidth(24).setHeight(26).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("eri_jump_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 20;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(31).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("eri_shoot_body.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(100).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(22).setReversed(true);
			gapFactor = 18;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("eri_final_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(125).setPadding(0).setLoop(false);
			animation.setWidth(48).setHeight(41).setReversed(true);
			gapFactor = 45;
			legFactor = 10;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

public:
	Eri(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		setAnimation(state);
		scale.x = scale.y = 2.5;
		gapFactor = 32;
		legFactor = 10;
		previousState = state;
	}
	void attack() override {

	}
	void takeDamage(int damage) override {

	}
};


