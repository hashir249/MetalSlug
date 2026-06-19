#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "AudioManager.h"
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
	sf::Vector2f position{};
	sf::IntRect hitbox{};
	bool active;  // updates entity
	int direction; // right or left ( 1 for right . 2 for left) 0 means direction does not matter
	int previousState; // for keeping track to not load same animation over and over again
	int state; // for rendering to know which state we are in (meaning differnte for each entity)
	int maxStates;
	bool onGround;
	sf::Vector2f velocity;
	AnimationManager animation;
	TextureManager* textureManager;
	AudioManager* audioManager;
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

	void setAudioManager(AudioManager* aud) {
		if (audioManager == nullptr) audioManager = aud;
	}

	Entity(TextureManager* tex, AudioManager* aud, int x, int y) : textureManager(tex), active(true) {
		position = sf::Vector2f(x, y);
		state = maxStates = 0;
		velocity = sf::Vector2f(0, 0);
		scale = sf::Vector2f(0, 0);
		hide = isEnemy = isProjectile;
		gravityEffect = true;
		product = nullptr;
		audioManager = nullptr;
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

	Entity& setCollided(bool c) {
		collided = c;
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
		this->position = sf::Vector2f(x, y);
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
	Entity& setVelocity(sf::Vector2f v) {
		this->velocity = v;
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
		if (state >= 0 && maxStates > 0) {
			if(maxStates > 0) this->state = (state) % maxStates;
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

	virtual Entity* productEntity() {
		Entity* product = this->product;
		this->product = nullptr;
		return product;
	}

	bool getEnemyStatus() const {
		return isEnemy;
	}
	bool getProjectileStatus() const {
		return isProjectile;
	}

	bool getCollided() const {
		return collided;
	}

	// only for enemies, the target locations of players are gonna be sent
	virtual void setTarget(sf::Vector2f pos) {

	}
	virtual void setProjectile(sf::Vector2f pos) {

	}
	virtual void setEnemyFlag(bool f) {

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