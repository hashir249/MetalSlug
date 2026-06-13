#pragma once

#include "Entity.h"

class Player;

class Collectible : public Entity {
protected:
	bool pickedUp;
public:
	Collectible(TextureManager* t,AudioManager* aud, int x = 0, int y = 0) : Entity(t, aud,x, y) {
		pickedUp = false;
		active = true;
		state = 0;
	}

	virtual void hitBoxUpdate() override {
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
		hitbox.height = actualHeight;
	}

	void interact(Entity* other) override {
		other->interactWithCollectible(this);
	}
	void handleInput() override {

	}

	virtual void interactWithPlayer(PlayerSoldier*) override {}
	virtual void interactWithEnemy(Enemy*) override {}
	virtual void interactWithProjectile(Projectile*) override {}
	virtual void interactWithVehicle(Vehicle*) override {}
	virtual void interactWithCollectible(Collectible*) override {}
	virtual void interactWithTerrain(Block*) override {}

	bool isPickedUp() const {
		return pickedUp;
	}
};


class Turkey : public Collectible {
	int statGiven;
	void setAnimation(int state) override {
		animation.setTexture(textureManager->getTexture("turkey.png"));
		animation.setTotalFrames(11).setCurrentFrame(0).setDelay(250).setLoop(false);
		animation.setWidth(31).setHeight(32);
	}
public:
	Turkey(TextureManager* tex,AudioManager* aud, int x, int y) : Collectible(tex,aud, x, y) {
		maxStates = 1; // only being displayed -> there is no other state for it
		state = 1;
		velocity.x = velocity.y = 0;
		onGround = false;
		scale.x = scale.y = 2;
		gravityEffect = true;
		statGiven = 3;
		direction = 2; // direction = 0 shows that direction is independent
		setAnimation(0);
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	void update() override {
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}

	void handleInput() override {

	}

	void interactWithPlayer(PlayerSoldier* p) override {
		if (Keyboard::isKeyPressed(sf::Keyboard::E)) {
			p->giveStat(statGiven);
			active = false;
		}
	}
};


class Fruit : public Collectible {
	int statGiven;
	void setAnimation(int state)  override {
		animation.setTexture(textureManager->getTexture("fruit.png"));
		animation.setTotalFrames(4).setCurrentFrame(0).setDelay(250).setLoop(true);
		animation.setWidth(31).setHeight(23);
	}
public:
	Fruit(TextureManager* tex,AudioManager* aud, int x, int y) : Collectible(tex,aud, x, y) {
		maxStates = 1; // only being displayed -> there is no other state for it
		state = 1;
		velocity.x = velocity.y = 0;
		scale.x = scale.y = 2;
		onGround = false;
		direction = 2; 
		setAnimation(0);
		statGiven = 2;
		gravityEffect = true;
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	void update() override {
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}
	void handleInput() override {

	}

	void interactWithPlayer(PlayerSoldier* p) override {
		if (Keyboard::isKeyPressed(sf::Keyboard::E)) {
			p->giveStat(statGiven);
			active = false;
		}
	}
};

class SupplyCrate : public Collectible {
private:
	void setAnimation(int state) override {
		animation.setTexture(textureManager->getTexture("supply_crate.png"));
		animation.setTotalFrames(6).setCurrentFrame(0).setDelay(250).setLoop(true);
		animation.setWidth(37).setHeight(32);
	}
public:
	SupplyCrate(TextureManager* tex,AudioManager* aud,int x, int y) : Collectible(tex,aud, x, y) {
		maxStates = 1; // only being displayed -> there is no other state for it
		state = 1;
		velocity = sf::Vector2f(0, 0);
		scale.x = scale.y = 2;
		onGround = true;
		direction = 2; // direction = 0 shows that direction is independent
		setAnimation(0);
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	void handleInput() override {

	}
	void update() override {
		animation.apply(sprite);
		animation.cycle();
		hitBoxUpdate();
	}

	void interactWithPlayer(PlayerSoldier* p) override {
		if (Keyboard::isKeyPressed(sf::Keyboard::E)) {
			active = false;
		}
	}

	~SupplyCrate() {

	}
};