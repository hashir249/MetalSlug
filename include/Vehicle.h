#pragma once

#include "DamageableEntity.h"

class Vehicle : public DamageableEntity {
protected:
	bool driverRequirement;
	Entity* driver;
	int fireRate;
	Clock fireTimer;
public:

	Vehicle(TextureManager* tex,AudioManager*aud, int x, int y) : DamageableEntity(tex,aud, x, y) {
		driver = nullptr;
	}
	virtual void drive() = 0;
	virtual void fire() = 0;

	virtual void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	bool occupied() const {
		return driver != nullptr;
	}

	void mount(Entity* p) {
		driver = p;
	}

	void unmount() {
		driver = nullptr;
	}

	// In Vehicle base class:
	void interactWithPlayer(PlayerSoldier* player) override {
		if (!driverRequirement) return;  // enemy vehicle, ignore

		if (driver == nullptr) {
			
		}
	}

};

class AerialVehicle : public Vehicle {
protected:
	float speed;
	float altitude;
	float verticalSpeed;

public:
	AerialVehicle(TextureManager* tex,AudioManager*aud, int x, int y) : Vehicle(tex,aud, x, y) {
		altitude = 0;
		//maxAltitude = 500.0f;
		//minAltitude = 0.0f;
		verticalSpeed = 2.0f;
		gravityEffect = true;
	}

	virtual void ascend() = 0;
	virtual void descend() = 0;

	void drive() override {

	}

	virtual void fire() = 0;
};


class FlyingTara : public AerialVehicle {
private:
	void setAnimation(int state) {
		if (state == 0) { // idle and simple moving
			animation.setTexture(textureManager->getTexture("tara_simple.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(83).setHeight(36);
		}
		else if (state == 1) { // tara giong up
			animation.setTexture(textureManager->getTexture("tara_flying_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(81).setHeight(46).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("tara_flying_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(400).setPadding(0).setLoop(false);
			animation.setWidth(82).setHeight(41).setReversed(true);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("tara_destroyed.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(80).setHeight(56).setReversed(true);
		}
	}
public:
	FlyingTara(TextureManager* tex,AudioManager* aud, int x, int y) : AerialVehicle(tex,aud, x, y) {
		maxStates = 4;
		state = 0;
		verticalSpeed = 5;
		speed = 9;
		direction = 1;
		scale.x = 2.5;
		scale.y = 2.5;
		driverRequirement = false;
		setAnimation(0);
	}

	void interact(Entity* e) override {

	}

	void fire() override {

	}


	void takeHit() override {

	}

	void takeDamage(int damage) override {

	}

	void ascend() override {

	}

	void descend() override {

	}

	void handleInput() override {
		return; // for flying tara input is not handled
		velocity.y = velocity.y = 0;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			state = 1;
			velocity.y = -speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = 0;
			velocity.x = speed;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = 0;
			velocity.x = -speed;
			direction = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = 2;
			velocity.y = speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			state = 3;
		}
	}

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


};


class SlugFlyer : public AerialVehicle {
private:
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("slugflyer_hatch_open.png"));
			animation.setCurrentFrame(0).setStartingFrame(4).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(82).setHeight(64);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("slugflyer_hatch_open.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(81).setHeight(64);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("slugflyer_moving.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(175).setPadding(0).setLoop(false);
			animation.setWidth(81).setHeight(55).setReversed(true);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("slugflyer_destroyed.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(80).setHeight(56);
		}

	}
public:
	SlugFlyer(TextureManager* tex,AudioManager* aud, int x, int y) : AerialVehicle(tex,aud, x, y) {
		/* 0 -> idle with hatch open
		*  1 -> hatch open and close
		*  2-> moving
		*  3 -> destroyed
		*/
		scale.x = 2.5;
		scale.y = 2.5;
		speed = 8;
		verticalSpeed = 4;
		state = 0;
		gravityEffect = true;
		driverRequirement = true;
		maxStates = 4;
		driver = nullptr;
		direction = 1; // right
		setAnimation(0);
	}

	void handleInput() override {
		if (driver == nullptr) return;
		velocity.y = velocity.y = 0;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			state = 2;
			velocity.y = -speed;
			velocity.x = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = 2;
			velocity.x = speed;
			velocity.y = 0;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = 2;
			velocity.x = -speed;
			direction = 2;
			velocity.y = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = 2;
			velocity.y = speed;
			velocity.x = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			state = 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			state = 1;
		}

		if (velocity.x == 0 && velocity.y == 0 && state == 2) {
			state = 0;
		}
	}

	void fire() override {

	}


	void takeHit() override {

	}

	void takeDamage(int damage) override {

	}

	void ascend() override {

	}

	void descend() override {

	}


	void update() override {
		//handleInput();
		hitBoxUpdate();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
			setVelocityX(0).setVelocityY(0);
		}
		animation.apply(sprite);
		animation.cycle();

	}

	void interact(Entity* e) override {

	}

	//void render(RenderWindow& window, int scroll_x, int scroll_y) override {
	//	sprite.setScale(2.5 * (direction == 1 ? 1 : -1), 2.5);
	//	sprite.setPosition(position.x - scroll_x + animation.getWidth(), position.y - scroll_y);
	//	window.draw(sprite);
	//}
};

class AquaticVehicle : public Vehicle {
protected:
	bool submerged;
	float depth;
	float diveSpeed;
	float surfaceSpeed;
	float underwaterSpeed;
public:
	AquaticVehicle(TextureManager* tex,AudioManager* aud, int x, int y) : Vehicle(tex,aud,x, y) {
		onGround = false;
		gravityEffect = false;
	}
	virtual void submerge() = 0;
	virtual void surfacing() = 0;
};


class Submarine : public AquaticVehicle {
private:
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("sub_simple.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(7).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(100).setHeight(48).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("sub_hit.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(100).setHeight(50);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("sub_destroyed.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(175).setPadding(0).setLoop(true);
			animation.setWidth(104).setHeight(60);
		}
	}
public:
	Submarine(TextureManager* tex,AudioManager* aud, int x, int y) : AquaticVehicle(tex,aud, x, y) {
		maxStates = 3;
		state = 0;
		direction = 1;
		setAnimation(state);
		scale.x = 2;
		scale.y = 2;
		submerged = false;
		driverRequirement = true;
		depth = 0;
		driverRequirement = false;
		underwaterSpeed = 3;
		surfaceSpeed = 5;
		diveSpeed = 2;
		previousState = state;
	}

	void handleInput() override {
		return;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			position.y -= diveSpeed;
			state = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			position.y += diveSpeed;
			state = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (submerged) position.x += underwaterSpeed;
			else velocity.x = surfaceSpeed;
			state = 0;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (submerged) position.x -= underwaterSpeed;
			else velocity.x = -surfaceSpeed;
			state = 0;
			direction = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			this->state = 2;
		}
	}

	void submerge() override {

	}

	void surfacing() override {
		velocity.x = 0;
		velocity.y = diveSpeed;
	}


	void update() override {
		velocity.y = velocity.y = 0;
		handleInput();
		if (previousState != state) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();

		;
		position.y += velocity.y;
		hitBoxUpdate();
	}

	void interact(Entity* other) override {

	}

	void drive() {
		handleInput();
	}

	void takeHit()override {
		velocity.x -= 0.1;
		velocity.y -= 0.1;
	}

	void fire() override {
		return;
	}

	void takeDamage(int damage) override {
		if (!invincible) hp -= damage;
	}
};


class SlugMariner : public AquaticVehicle {
private:
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("mariner_hatch_open.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(1).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(64);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("mariner_hatch_open.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(59).setHeight(64);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("mariner_idle_straight.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(58).setHeight(53).setReversed(true);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("mariner_up.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(60).setHeight(52);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("mariner_down.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setLoop(true);
			animation.setWidth(56).setHeight(55);
		}

	}
public:
	SlugMariner(TextureManager* tex,AudioManager* aud, int x = 0, int y = 0) : AquaticVehicle(tex,aud, x, y) {
		state = 1;
		maxStates = 4; // state(0) -> idle with hatch open , state(1) -> hatchopejing or close depending on player leaving or entering , state(2) -> straightMovement
		// state(3) -> marine going up , state(4) ->marine going down 
		setAnimation(state);
		submerged = false;
		driverRequirement = true;

		previousState = state;
		driverRequirement = true;
		// for now no special intended meaning used
		depth = 0;
		underwaterSpeed = 1.5;
		surfaceSpeed = 2.5;
		diveSpeed = 1;

		scale.x = 2.75;
		scale.y = 2.75;
	}

	void interact(Entity* other) override {

	}

	void handleInput() override {
		if (driver == nullptr) return;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			position.y -= diveSpeed;
			state = 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			position.y += diveSpeed;
			state = 4;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (submerged) position.x += underwaterSpeed;
			else velocity.x = surfaceSpeed;
			state = 2;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (submerged) position.x -= underwaterSpeed;
			else velocity.x = -surfaceSpeed;
			state = 2;
			direction = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			this->state = 1;
		}
	}

	void submerge() override {

	}

	void surfacing() override {
		velocity.x = 0;
		velocity.y = diveSpeed;
	}


	void update() override {
		handleInput();
		if (previousState != state) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}


	void drive() {
		handleInput();
	}

	void takeHit()override {
		velocity.x -= 0.1;
		velocity.y -= 0.1;
	}

	void fire() override {
		return;
	}

	void takeDamage(int damage) override {
		if (!invincible) hp -= damage;
	}
};


class GroundVehicle : public Vehicle {
protected:
	float speed;
	float step;
	int turretAngle;  // 0-90 degrees per project spec

public:
	GroundVehicle(TextureManager* tex,AudioManager* aud, int x, int y) : Vehicle(tex,aud, x, y) {
		speed = 2.0f;
		step = 1;
		turretAngle = 0;
		gravityEffect = true;
	}

	void drive() override {
		position.x;
		position.y += velocity.y;
	}

	void rotateTurretUp() {
		turretAngle += 5;
		if (turretAngle > 90) turretAngle = 90;
	}

	void rotateTurretDown() {
		turretAngle -= 5;
		if (turretAngle < 0) turretAngle = 0;
	}

	virtual void fire() = 0;
};


class MetalSlug : public GroundVehicle {
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("metalslug_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(62).setHeight(56);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("metalslug_straight.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(false).setReversed(true);
			animation.setWidth(65).setHeight(55);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("metalslug_angled.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(10).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(60).setHeight(55);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("metalslug_uphill.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(60).setHeight(57);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("metalslug_uphill_angled.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(60).setHeight(57);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("metalslug_downhill.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(60).setHeight(57);
		}
		else if (state == 6) {
			animation.setTexture(textureManager->getTexture("metalslug_downhill_angled.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(56).setHeight(63);
		}
		else if (state == 7) {
			animation.setTexture(textureManager->getTexture("metalslug_destroyed.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setPadding(0).setLoop(false);
			animation.setWidth(62).setHeight(63);
		}
	}
private:
public:
	MetalSlug(TextureManager* tex, AudioManager* aud, int x, int y) : GroundVehicle(tex,aud, x, y) {
		/*  0 -> idle | 1 -> straight move | 2 -> straightAngled | 3 -> uphill simple
		*	4 -> uphill Angled | 5 -> downhill | 6 -> downhillangled | 7 -> destroyed
		*/
		scale.x = 2.5;
		scale.y = 2.5;
		maxStates = 8;
		state = 0;
		step = 1;
		speed = 7;
		direction = 1;
		driverRequirement = true;
		setAnimation(0);
	}

	void handleInput() override {
		if (driver == nullptr) return;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {

		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {

		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			velocity.x = speed;
			state = 1;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			velocity.x = -speed;
			state = 1;
			direction = 2;
		}
	}

	void update() override {
		velocity.y = velocity.y = 0;
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();

		hitBoxUpdate();
	}

	void takeDamage(int damage) override {

	}

	void takeHit() override {

	}

	void interact(Entity* other) override {

	}

	void fire() override {

	}
};


class Bradley : public GroundVehicle {
private:

	bool launchReady;
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("bradley_lower_move.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(81).setHeight(52).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("bradley_launcher_raise.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(81).setHeight(65).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("bradley_raised_move.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(7).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(82).setHeight(78);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("bradley_starting_turn.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(true);
			animation.setWidth(78).setHeight(79);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("bradley_turning.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setPadding(0).setLoop(true);
			animation.setWidth(55).setHeight(79);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("bradley_ending_turn.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(true);
			animation.setWidth(78).setHeight(65);
		}
		else if (state == 6) {
			animation.setTexture(textureManager->getTexture("bradley_wrecked.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setPadding(0).setLoop(true);
			animation.setWidth(112).setHeight(73);
		}
	}
public:
	Bradley(TextureManager* tex, AudioManager* aud, int x, int y) : GroundVehicle(tex,aud, x, y) {
		step = 2;
		speed = 6;
		scale.x = 2.5;
		scale.y = 2.5;
		state = 0;
		launchReady = false;
		maxStates = 7;
		driverRequirement = false;
		direction = 1;
		active = true;
		/* 0 -> bradley with launcher lower
		*  1 -> launcher raise
		*  2 -> moving with launcher raised
		*  3 -> starting turn
		*  4 -> in middle of turning
		*  5 -> ending turn
		*  6 -> wrecked
		*/
		previousState = state;
		setAnimation(state);
	}

	void handleInput() override {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			launchReady = true;
			state = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			launchReady = false;
			state = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			velocity.x = speed;
			state = (launchReady) ? 2 : 0;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			velocity.x = -speed;
			state = (launchReady) ? 2 : 0;
			direction = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			state = 1;
		}
	}

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

	void takeDamage(int damage) override {

	}

	void takeHit() override {

	}

	void fire() override {

	}
};