#pragma once
#include "Entity.h"
#include <cmath>

class Projectile : public Entity {
protected:
	int angle;
	int impactRadius;
	int damage;
	float speed; // total overall speed, the velocityX and velocityY weuld be at one instant
public:
	Projectile(TextureManager* tex,AudioManager* aud, int x, int y) : Entity(tex,aud, x, y) {
		active = true;
		isProjectile = true;
	}

	void handleInput() override {
		return; // no work to be done in handleInput as they are directed from start and independently go
	}

	int getDamage() override {
		return damage;
	}

	void interact(Entity* other) override {
		other->interactWithProjectile(this);
	}

	void interactWithEnemy(Enemy* e) override {

	}

	virtual void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	virtual ~Projectile() {}
};