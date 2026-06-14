#pragma once

#include "DamageableEntity.h"
#include "EnemyAIState.h"
#include "Inventory.h"

class Enemy : public DamageableEntity {
protected:
	EnemyAIState enemyAI;
	float speed;
	Inventory inventory;
public:
	Enemy(TextureManager* tex, AudioManager* aud, int x, int y) : DamageableEntity(tex, aud,x, y) {
		gravityEffect = true;
		isEnemy = true;
	}
	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
	}

	void interact(Entity* other) override {
		other->interactWithEnemy(this);
	}

	void interactWithProjectile(Projectile* p) override {
		takeDamage(p->getDamage());
	}

};

class GrenadeSoldier : public Enemy {
private:
	void setAnimation(int state) {

	}
public:

};

class RebelSoldier : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("rebel_soldier_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(500).setPadding(0).setLoop(false);
			animation.setWidth(50).setHeight(51).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("rebel_soldier_running.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(100).setPadding(0).setLoop(true);
			animation.setWidth(51).setHeight(51).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("rebel_soldier_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(10).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(59).setHeight(51);
		}
	}

public:
	RebelSoldier(TextureManager* tex,AudioManager* aud,  int x, int y) : Enemy(tex,aud, x, y) {
		maxStates = 3;
		state = 0;
		previousState = 0;
		scale.x = 1.8;
		scale.y = 2.1;
		direction = 1;
		speed = 4;
		active = true;
		setAnimation(state);
		// now setting the inventoy stats 
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);

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

class BazookaSoldier : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(41).setHeight(45).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_running.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(43).setHeight(41).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_jumping.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(43).setHeight(44);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(true);
			animation.setWidth(41).setHeight(42);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_recoil.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(true);
			animation.setWidth(40).setHeight(44);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_die.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(60).setHeight(57);
		}
	}
public:
	BazookaSoldier(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex,aud, x, y) {
		/* 0 -> idle, 1 -> running , 2 ->  jump , 3 -> shoot, 4 -> recoil , 5 -> die
		*/
		maxStates = 6;
		state = 0;
		previousState = 0;
		scale.x = 2.5;
		scale.y = 2.5;
		direction = 1;
		speed = 2;
		active = true;
		setAnimation(state);

		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);

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
		hitBoxUpdate();
	}

};

class ShieldedSoldier : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(36).setHeight(39).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_running.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(40);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(39).setHeight(43);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(1000).setPadding(0).setLoop(false);
			animation.setWidth(38).setHeight(38);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_up_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(42).setHeight(49);
		}
		else if (state == 5) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_die.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setPadding(0).setLoop(false);
			animation.setWidth(60).setHeight(57);
		}
	}
public:
	ShieldedSoldier(TextureManager* tex,AudioManager* aud,  int x, int y) : Enemy(tex,aud, x, y) {
		/* 0 -> idle, 1 -> running , 2 ->  jump , 3 -> shoot, 4 -> upshoot , 5 -> die
		*/
		maxStates = 6;
		state = 0;
		previousState = 0;
		scale.x = 2.5;
		scale.y = 2.5;
		direction = 1;
		speed = 2;
		active = true;
		setAnimation(state);

		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);

		//inventory.addWeapon(new Pistol(textureManager, weaponX, weaponY, direction, 0, false));
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
		hitBoxUpdate();
	}

};

class MummyWarrior : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(7).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(35).setHeight(43).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_walk.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(11).setDelay(150).setPadding(0).setLoop(false);
			animation.setWidth(40).setHeight(45);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(47).setHeight(48);
		}
	}
public:
	MummyWarrior(TextureManager* tex,AudioManager* aud, int x, int y) : Enemy(tex,aud, x, y) {
		/* 0 -> idle, 1 -> running , 2 ->  die(fire)
		*/
		maxStates = 3;
		state = 0;
		previousState = 0;
		scale.x = 2.5;
		scale.y = 2.5;
		direction = 1; // 1 - > right, 2 -> left
		speed = 3;
		active = true;
		setAnimation(state);
	}

	void handleInput() override {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			state = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = 2;
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
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			state = 2;
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

class Zombie : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(53).setHeight(42).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_walk.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(40).setHeight(41).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(45).setHeight(45);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_hit.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(1000).setPadding(0).setLoop(false);
			animation.setWidth(31).setHeight(57);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_die.png"));
			animation.setCurrentFrame(0).setStartingFrame(3).setTotalFrames(6).setPadding(0).setLoop(false);
			animation.setWidth(19).setHeight(49);
		}
	}
public:
	Zombie(TextureManager* tex,AudioManager*aud, int x, int y) : Enemy(tex,aud, x, y) {
		/* 0 -> idle, 1 -> walk, 2 -> shoot, 3 - >hit, 4 -> die
		*/
		maxStates = 5;
		state = 0;
		previousState = 0;
		scale.x = 2.5;
		scale.y = 2.5;
		direction = 1; // 1 - > right, 2 -> left
		speed = 5;
		active = true;
		velocity.x = 0;
		velocity.y = 0;
		setAnimation(state);
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


class Martian : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("martian_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(53).setHeight(42).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("martian_running.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(40).setHeight(41).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("martian_shooting.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(45).setHeight(45);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("martian_fire_damage.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(17).setDelay(1000).setPadding(0).setLoop(false);
			animation.setWidth(31).setHeight(57);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("marian_projectile_damage.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setPadding(0).setLoop(false);
			animation.setWidth(19).setHeight(49);
		}
	}
public:
	Martian(TextureManager* tex,AudioManager* aud, int x, int y) : Enemy(tex,aud, x, y) {
		/* 0 -> idle, 1 -> walk, 2 -> shoot, 3 - >hit, 4 -> die
		*/
		maxStates = 5;
		state = 0;
		previousState = 0;
		scale.x = 2.5;
		scale.y = 2.5;
		direction = 2; // 1 - > right, 2 -> left
		speed = 5;
		active = true;
		velocity.x = 0;
		velocity.y = 0;

		setAnimation(state);
	}



	void handleInput() override {
		
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