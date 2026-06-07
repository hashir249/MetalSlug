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
		maxStates = 9;
		state = 0;
		speed = 7.0f;
		direction = 1;
		scale.x = 2.0;
		scale.y = 2.0;
		gapFactor = 0;
		saturationStat = 50;
		legFactor = 0;
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		if (this->hide == true) return;
		sprite.setPosition((float)position.x - scroll_x, (float)position.y - scroll_y);
		sprite.setScale(direction == 2 ? -scale.x : scale.x, scale.y);
		window.draw(sprite);
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
	virtual void update() {}
	virtual void handleInput() {}
};


class Tarma : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("tarma_body_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(21).setHeight(16).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("tarma_body_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(35).setHeight(30).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("tarma_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(31).setHeight(20).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("tarma_body_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(30).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("tarma_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("tarma_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(29).setHeight(24).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(21).setHeight(16).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("tarma_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(30).setHeight(42).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("tarma_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(21).setHeight(16).setReversed(true);
		}
	}

public:
	Tarma(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		direction = 1;
		setAnimation(state);
		scale.x = 2.5;
		scale.y = 2.5;
		onGround = false;
		gravityEffect = true;
		speed = 5;
		jumpForce = 15;
		previousState = state;
		gapFactor = 16;
		hitBoxUpdate();
	}

	void handleInput() override {

		velocity.x = 0;
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			attack();
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			
		}

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
			state = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up) && onGround) {
			velocity.y = -jumpForce;
			onGround = false;
			state = 2;
		}
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

	void attack() override {
		//takeShot(position.x + (direction == 1 ? 30 : -30), position.y - 20);
	}

	void takeDamage(int damage) override {

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

	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		if (this->hide == true) return;
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		legs.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y);
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		legs.setPosition(position.x + (direction == 1 ? legFactor : -legFactor) - scroll_x + 0.5 * (animation.getWidth() * scale.x), (scale.y * animation.getHeight()) + position.y - scroll_y - gapFactor); // the y position woudl eb y posioint of bdoy plys
		window.draw(legs);
		window.draw(sprite);
	}
};


class Marco : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("marco_body_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(30).setHeight(30).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("marco_body_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(28).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("marco_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(25).setHeight(20).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("marco_body_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(30).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("marco_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(33).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("marco_body_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(50).setHeight(24).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("marco_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(50).setHeight(24).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("marco_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(22).setHeight(16).setReversed(true);
		}
	}

public:
	Marco(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 4;
		state = 0;
		direction = 1;
		setAnimation(state);
		scale.x = 2.5;
		scale.y = 2.5;
		onGround = false;
		gravityEffect = true;
		speed = 5;
		jumpForce = 20;
		previousState = state;
		gapFactor = 12;
		legFactor = 3;

	}

	void handleInput() override {

	}
	void attack() override {
		//takeShot(position.x + (direction == 1 ? 30 : -30), position.y - 20);
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



	void takeDamage(int damage) override {

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

	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		if (this->hide == true) return;
		if (this->hide == true) return;
		if (inVehicle()) return; // when in vehicle then no renderig
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		legs.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y);
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		legs.setPosition(position.x + (direction == 1 ? legFactor : -legFactor) - scroll_x + 0.5 * (animation.getWidth() * scale.x), (scale.y * animation.getHeight()) + position.y - scroll_y - gapFactor); // the y position woudl eb y posioint of bdoy plys
		window.draw(legs);
		window.draw(sprite);
	}
};


class Fiolina : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("fiolina_body_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(29).setReversed(true);
			gapFactor = 30;
			legFactor = 5;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("fiolina_body_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(31).setHeight(28).setReversed(true);
			gapFactor = 30;
			legFactor = 5;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(29).setHeight(26).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("fiolina_body_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 15;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(27).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("fiolina_body_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(52).setHeight(22).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("fiolina_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("fiolina_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(41).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

public:
	Fiolina(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		direction = 1;
		setAnimation(state);
		scale.x = 2.5;
		scale.y = 2.5;
		onGround = false;
		gravityEffect = true;
		speed = 5;
		jumpForce = 16;

		gapFactor = 38;
		legFactor = 5;
		previousState = state;
	}

	void handleInput() override {

	}
	void attack() override {
		
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



	void takeDamage(int damage) override {

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

	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		if (this->hide == true) return;
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		legs.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y);
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		legs.setPosition(position.x + (direction == 1 ? legFactor : -legFactor) - scroll_x + 0.5 * (animation.getWidth() * scale.x), (scale.y * animation.getHeight()) + position.y - scroll_y - gapFactor); // the y position woudl eb y posioint of bdoy plys
		window.draw(legs);
		window.draw(sprite);

	}
};


class Eri : public PlayerSoldier {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle
			animation.setTexture(textureManager->getTexture("eri_body_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(29).setReversed(true);
			gapFactor = 30;
			legFactor = 10;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 1) { // running
			animation.setTexture(textureManager->getTexture("eri_body_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(28).setReversed(true);
			gapFactor = 30;
			legFactor = 10;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_run.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(23).setHeight(26).setReversed(true);
		}

		else if (state == 2) { // jumping
			animation.setTexture(textureManager->getTexture("eri_body_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(34).setHeight(25).setReversed(true);
			gapFactor = 20;
			legFactor = 0;
			animationLegs.setTexture(textureManager->getTexture("eri_legs_jump.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animationLegs.setWidth(31).setHeight(22).setReversed(true);
		}

		else if (state == 3) { // shooting
			animation.setTexture(textureManager->getTexture("eri_body_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(52).setHeight(22).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("eri_melee.png"));
			animation.setCurrentFrame(0).setStartingFrame(2).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(46).setHeight(41).setReversed(true);

			animationLegs.setTexture(textureManager->getTexture("eri_legs_idle.png"));
			animationLegs.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animationLegs.setWidth(20).setHeight(24).setReversed(true);
		}
	}

public:
	Eri(TextureManager* tex, int x, int y) : PlayerSoldier(tex, x, y) {
		maxStates = 5;
		state = 0;
		direction = 1;
		setAnimation(state);
		scale.x = 2.5;
		scale.y = 2.5;
		onGround = false;
		gravityEffect = true;
		speed = 5;
		jumpForce = 16;

		gapFactor = 32;
		legFactor = 10;
		previousState = state;

	}

	void handleInput() override {
	}
	void attack() override {

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


	void takeDamage(int damage) override {

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

	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		if (this->hide == true) return;
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		legs.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y);
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		legs.setPosition(position.x + (direction == 1 ? legFactor : -legFactor) - scroll_x + 0.5 * (animation.getWidth() * scale.x), (scale.y * animation.getHeight()) + position.y - scroll_y - gapFactor); // the y position woudl eb y posioint of bdoy plys
		window.draw(legs);
		window.draw(sprite);

	}
};


