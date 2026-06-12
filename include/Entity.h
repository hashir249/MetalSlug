#pragma once

#include "SFML/Graphics.hpp"
#include "TextureManager.h"
#include "AnimationManager.h"
class Vehicle;
class CollisionManager;
class Projectile;
class Enemy;
class PlayerSoldier;
class Collectible;

//#include "CollisionManager.h"
#include "Block.h"
using namespace sf;

class Entity {
protected:
	sf::Vector2f position;
	sf::IntRect hitbox;
	bool active;  // updates entity
	int direction; // right or left ( 1 for right . 2 for left) 0 means direction does not matter
	int previousState; // for keeping track to not load same animation over and over again
	int state; // for rendering to know which state we are in (meaning differnte for each entity)
	int maxStates;
	bool onGround;
	sf::Vector2f velocity;
	AnimationManager animation;
	TextureManager* textureManager;
	Sprite sprite;
	sf::Vector2f scale;
	bool gravityEffect;
	bool collided;
	bool hide;
	Entity* product;
	bool isProjectile;
	bool isEnemy;
public:
	virtual void hitBoxUpdate() {
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

	void updateCollision(bool option) {
		this->collided = option;
	}

	Entity(TextureManager* tex, int x, int y) : textureManager(tex) {
		position.x = x;
		position.y = y;
		state = 0;
		maxStates = 0;
		velocity.x = velocity.y = 0;
		hide = false;
		active = true;
		scale.x = scale.y = 1;
		gravityEffect = true;
		product = nullptr;
	}

	// main virtual functions =
	virtual void render(RenderWindow&, int, int) = 0;
	virtual void update() = 0;
	virtual void handleInput() = 0;


	// setters and getters
	Entity& setHitBox(sf::IntRect& hitbox) {
		this->hitbox = hitbox;
		return *this;
	}

	void setHide(bool option) {
		this->hide = option;
	}

	Entity& setActive(bool active) {
		this->active = active;
		return *this;
	}

	Entity& setPosition(int left, int top, int width, int height) {
		hitbox.left = left;
		hitbox.top = top;
		hitbox.width = (width < 0) ? 0 : width;
		hitbox.height = (height < 0) ? 0 : height;

		return *this;
	}

	Entity& setPosition(sf::Vector2f& position) {
		this->position = position;

		return *this;
	}

	bool getIsEnemy() const {
		return isEnemy;
	}

	bool getIsProjectile() const {
		return isProjectile;
	}

	Entity& setOnGround(bool state) {
		this->onGround = state;
		return *this;
	}

	Entity& setPosition(int x, int y) {
		this->position.x = x;
		this->position.y = y;

		return *this;
	}

	Entity& setVelocityX(float x) {
		velocity.x = x;
		return *this;
	}
	Entity& setVelocityY(float y) {
		velocity.y = y;
		return *this;
	}

	Entity& setMaxStates(int state) {
		if (state >= 0) {
			this->maxStates = state;
		}
		return *this;
	}

	bool getActive() const {
		return active;
	}

	Entity& setState(int state) {
		if (state >= 0) {
			this->state = (state) % maxStates;
		}

		return *this;
	}

	Entity& setDirection(int direction) {
		if (direction == 1 || direction == 2) {
			this->direction = direction;
		}

		return *this;
	}


	// getters 
	sf::IntRect getHitBox() const {
		return hitbox;
	}

	int getDirection() const {
		return direction;
	}

	int getState() const {
		return state;
	}

	int getMaxStates() const {
		return maxStates;
	}

	virtual int getDamage() {
		return 0;
	}

	float getVelocityX() const {
		return velocity.x;
	}

	float getVelocityY() const {
		return velocity.y;
	}

	sf::Vector2f getVelocity() const {
		return velocity;
	}

	void virtual enterVehicle(Vehicle* v) {

	}

	void virtual setAnimation(int state) = 0;

	float getScaleX() const {
		return scale.x;
	}
	sf::Vector2f getScale() const {
		return scale;
	}
	float getScaleY() const {
		return scale.y;
	}

	const AnimationManager& getAnimation() const {
		return animation;
	}

	bool isGravityEffect() const {
		return gravityEffect;
	}

	int getX() const {
		return position.x;
	}
	sf::Vector2f getPosition() const {
		return position;
	}
	int getY() const {
		return position.y;
	}
	Entity& setGravityEffect(bool option) {
		gravityEffect = option;
		return *this;
	}

	virtual void giveStat(int amount) {

	}

	virtual Entity* productEntity() {
		return product;
	}

	// interactions for each object so right fuctions are dispatched
	
	// double dispatch pattern
	virtual void interact(Entity* other) = 0;
	virtual void interactWithPlayer(PlayerSoldier*) = 0;
	virtual void interactWithEnemy(Enemy*) = 0;
	virtual void interactWithProjectile(Projectile*) = 0;
	virtual void interactWithVehicle(Vehicle*) = 0;
	virtual void interactWithCollectible(Collectible*) = 0;
	virtual void interactWithTerrain(Block*) = 0;
};