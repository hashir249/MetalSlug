#pragma once

#include "DamageableEntity.h"
#include "PlayerSoldier.h"
using namespace sf;

class Vehicle : public DamageableEntity {
protected:
	bool driverRequirement;
	PlayerSoldier* driver;
public:
	Vehicle(TextureManager* tex,AudioManager*aud, int x, int y) : DamageableEntity(tex,aud, x, y) {
		driver = nullptr;
	}
	virtual void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}
	
	bool occupied() const;
	void mount(PlayerSoldier* p);
	void unmount();
	void takeDamage(int damage) override {

	}
	void takeHit() override {

	}
	// overrides
	virtual void interact(Entity* other) override {
		other->interactWithVehicle(this);
	}
	virtual void interactWithPlayer(PlayerSoldier* p) override;
	virtual void interactWithEnemy(Enemy*) override {}
	virtual void interactWithProjectile(Projectile*) override {}
	virtual void interactWithVehicle(Vehicle*) override {}
	virtual void interactWithCollectible(Collectible*) override {}
	virtual void interactWithTerrain(Block*) override {}
};

class GroundVehicle : public Vehicle {
protected:
	float speed;
	float step;
	int turretAngle;  // 0-90 degrees per project spec

public:
	GroundVehicle(TextureManager* tex, AudioManager* aud, int x, int y) : Vehicle(tex, aud, x, y) {
		speed = 5.0f;
		step = 1;
		gravityEffect = true;
	}

};


class MetalSlug : public GroundVehicle {
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("metalslug_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(500).setPadding(0).setLoop(true);
			animation.setWidth(63).setHeight(56);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("metalslug_straight.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(true).setReversed(true);
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
public:
	MetalSlug(TextureManager* tex, AudioManager* aud, int x, int y) : GroundVehicle(tex, aud, x, y) {
		/*  0 -> idle | 1 -> straight move | 2 -> straightAngled | 3 -> uphill simple
		*	4 -> uphill Angled | 5 -> downhill | 6 -> downhillangled | 7 -> destroyed
		*/
		scale = sf::Vector2f(2.5, 2.5);
		maxStates = 8;
		state = 0;
		step = 1;
		speed = 8;
		direction = 1;
		driverRequirement = true;
		setAnimation(0);
	}

	void handleInput() override {
		if (driver == nullptr) return;
		velocity.x = 0;
		
		if (Keyboard::isKeyPressed(Keyboard::Up)) {

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

		if (velocity == sf::Vector2f(0, 0)) state = 0;
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
	Bradley(TextureManager* tex, AudioManager* aud, int x, int y) : GroundVehicle(tex, aud, x, y) {
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
		verticalSpeed = 10;
		gravityEffect = true;
	}
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

	void handleInput() override {
		//return; // for flying tara input is not handled
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
			animation.setTexture(textureManager->getTexture("slugflyer_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(81).setHeight(53);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("slugflyer_hatch_open.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(81).setHeight(64);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("slugflyer_moving.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(175).setPadding(0).setLoop(true);
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
		scale = sf::Vector2f(2.5, 2.5);
		speed = 8;
		verticalSpeed = 4;
		state = 0;
		driverRequirement = true;
		maxStates = 4;
		driver = nullptr;
		direction = 1; // right
		setAnimation(state);
	}

	void handleInput() override {
		if (driver == nullptr) return;
		velocity.x = 0;
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			state = 2;
			velocity.y = -speed;
			//velocity.x = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = 2;
			velocity.x = speed;
			//velocity.y = 0;
			direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = 2;
			velocity.x = -speed;
			direction = 2;
			//velocity.y = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = 2;
			velocity.y = speed;
			//velocity.x = 0;
		}

		if (velocity.x == 0 && velocity.y == 0) {
			state = 0;
		}
	}

	void update() override {
		setVelocity(sf::Vector2f());
		handleInput();
		hitBoxUpdate();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
			setVelocity(sf::Vector2f());
		}
		animation.apply(sprite);
		animation.cycle();
	}
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

	void update() override {
		velocity.y = velocity.y = 0;
		handleInput();
		if (previousState != state) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();

		
		position.y += velocity.y;
		hitBoxUpdate();
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
		previousState = state;
		driverRequirement = true;
		underwaterSpeed = 4;
		surfaceSpeed = 7;
		diveSpeed = 3;
		scale = sf::Vector2f(2.75, 2.75);
	}


	void handleInput() override {
		if (driver == nullptr) return;
		velocity.x = velocity.y = 0;
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

	void update() override {
		handleInput();
		if (previousState != state) {
			setAnimation(state);
			previousState = state;
			velocity = sf::Vector2f();
		}
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}
};
