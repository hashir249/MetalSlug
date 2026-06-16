#pragma once

#include "DamageableEntity.h"
#include "EnemyAIState.h"
#include "Inventory.h"

class Enemy : public DamageableEntity {
protected:
	EnemyAIState enemyAI;
	float speed;
	Inventory inventory;
	sf::Vector2f weaponPlug;
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

	virtual void setTarget(sf::Vector2f pos) override {
		enemyAI.updateTarget(pos);
	}
	virtual void setProjectile(sf::Vector2f pos) override {
		enemyAI.updateProjectile(pos);
	}
	void takeDamage(int d) override {
		this->hp -= d;
	}
	void handleInput() override {}
	void takeHit() override {}

	virtual void interactWithPlayer(PlayerSoldier*) override {}
	virtual void interactWithEnemy(Enemy*) override {}
	virtual void interactWithProjectile(Projectile* p) override {
		cout << "Interaction" << endl;
		if (isDead()) return;
		if (!(p->getCollided() || p->isEnemyProjectile())) {
			takeDamage(p->getDamage());
			p->setCollided(true);
		}
		cout << "Damage: " << p->getDamage() << endl;
		cout << "HP: " << this->getHp() << endl;
	}
	virtual void interactWithVehicle(Vehicle*) override {}
	virtual void interactWithCollectible(Collectible*) override {}
	virtual void interactWithTerrain(Block*) override {}
	
	// making code modular by introducing functions
	virtual void InitialStats() = 0;
	virtual void PatrolState() {
		enemyAI.patrol(speed);
		direction = enemyAI.getPatrolDirection();
		velocity.x = (direction == 1) ? speed : -speed;
		state = 1;
	}
	virtual void MoveAndShoot(int i, int m, int s) {
		direction = enemyAI.directionSet(position);
		bool move = enemyAI.applyMovement(position);
		bool shoot = enemyAI.applyShooting(position);
		if (move) {
			velocity.x = (direction == 1) ? speed : -speed;
			state = m;
		}
		else if (shoot && !move) {
			product = inventory.fire();
			state = s;
		}
		else state = i;
	}

	virtual Entity* productEntity() override {
		Entity* product = this->product;
		if (product != nullptr) product->setEnemyFlag(true);
		this->product = nullptr;
		return product;
	}
};

class ShieldedSoldier : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(35).setHeight(39).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_running.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(32).setHeight(40);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setDelay(1000).setPadding(0).setLoop(false);
			animation.setWidth(38).setHeight(38);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("shielded_soldier_jump.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(39).setHeight(43);
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

	void InitialStats() {
		maxStates = 6, state = previousState = 0;
		scale.x = scale.y = 2.5;
		speed = 3;
		hp = 5;
		enemyAI.setRange(1000);
	}
public:
	ShieldedSoldier(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex, aud, x, y) {
		//0 -> idle, 1 -> running , 2 ->  jump , 3 -> shoot, 4 -> upshoot , 5 -> die
		InitialStats();
		setAnimation(state);
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);
		inventory.addWeapon(new ShotGun(textureManager, audioManager, weaponX, weaponY, direction, false));
	}
	void update() override {
		velocity.x = 0;
	
		if (state == 2 && animation.getDone()) {
			state = 0;
		}

		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory.update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}

	void handleInput() override {
		bool a = enemyAI.alert(position);
		if (a) {
			MoveAndShoot(0,1,2);
		}
		else {
			PatrolState();
		}
	}

	virtual void interactWithProjectile(Projectile* p) override {
		if (direction == p->getDirection()) return;
		if (!p->getCollided()) {
			takeDamage(p->getDamage());
			p->setCollided(true);
		}
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
			animation.setWidth(45).setHeight(43);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_hit.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(250).setPadding(0).setLoop(false);
			animation.setWidth(39).setHeight(63);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("zombie_enemy_die.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(2).setPadding(0).setDelay(400).setLoop(false);
			animation.setWidth(32).setHeight(63);
		}
	}

	void InitialStats() override {
		maxStates = 5, state = previousState = 0;
		scale.x = scale.y = 2.5;
		speed = 5;
		velocity = sf::Vector2f(0, 0);
		hp = 10;
		enemyAI.setRange(1000);
		weaponPlug = sf::Vector2f(26, 13);
	}
public:
	Zombie(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex, aud, x, y) {
		// 0 -> idle, 1 -> walk, 2 -> shoot, 3 - >hit, 4 -> die
		InitialStats();
		setAnimation(state);
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);
		inventory.addWeapon(new ShotGun(textureManager, audioManager, weaponX + 30, weaponY + 20, direction, true));
	}
	void update() override {
		velocity.x = 0;

		if (isDead()) {
			if (state != 3 && state != 4) {
				state = 3;
			}
			if (state == 3 && animation.getDone()) state = 4;
			if (state == 4 && animation.getDone()) active = false;
		}
		else {
			handleInput();
			if (state == 2 && animation.getDone()) {
				state = 0;
			}
		}

		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}

		animation.apply(sprite).cycle();
		hitBoxUpdate();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory.update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}


	void handleInput() override {
		bool a = enemyAI.alert(position);
		if (a) {
			MoveAndShoot(0,1,2);
		}
		else {
			PatrolState();
		}
	}
};

class MummyWarrior : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(7).setDelay(250).setPadding(0).setLoop(true);
			animation.setWidth(35).setHeight(43).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_walk.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(11).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(40).setHeight(45);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("mummy_warrior_fire.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(47).setHeight(48);
		}
	}

	void InitialStats() {
		maxStates = 3, state = previousState = 0;
		scale.x = scale.y = 2.5;
		speed = 3;
		enemyAI.setRange(1000);
	}
public:
	MummyWarrior(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex, aud, x, y) {
		InitialStats();
	}

	void handleInput() override {
		bool a = enemyAI.alert(position);
		if (a) {
			direction = enemyAI.directionSet(position);
			velocity.x = (direction == 1) ? speed : -speed;
			state = 1;   // walking
		}
		else {
			PatrolState();
		}
	}

	void update() override {
		velocity.x = 0;
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
		inventory.update(position, direction);
	}

};

class RebelSoldier : public Enemy {
private:
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("rebel_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setDelay(500).setPadding(0).setLoop(false);
			animation.setWidth(41).setHeight(38).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("rebel_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(12).setDelay(100).setPadding(0).setLoop(true);
			animation.setWidth(39).setHeight(37).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("rebel_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(9).setDelay(75).setPadding(0).setLoop(true);
			animation.setWidth(46).setHeight(47);
		}
	}
	void InitialStats() override {
		maxStates = 3, state = previousState = 0;
		scale = sf::Vector2f(2.5, 2.5);
		speed = 4;
		enemyAI.setRange(1000);
	}
public:
	RebelSoldier(TextureManager* tex,AudioManager* aud,  int x, int y) : Enemy(tex,aud, x, y) {
		InitialStats();
		setAnimation(state);
		// now setting the inventoy stats 
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);
		inventory.addWeapon(new ShotGun(textureManager, audioManager, x, y, direction, false));
	}

	void update() override {
		velocity.x = 0;
		if (isDead()) {
			active = false;
			return;
		}
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory.update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}

	void handleInput() override {
		bool a = enemyAI.alert(position);
		if (a) {
			MoveAndShoot(0, 1, 2);
		}
		else {
			PatrolState();
		}
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
			animation.setTexture(textureManager->getTexture("bazooka_soldier_shoot.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(false);
			animation.setWidth(41).setHeight(42);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_jumping.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(8).setDelay(150).setPadding(0).setLoop(true);
			animation.setWidth(43).setHeight(44);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("bazooka_soldier_recoil.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(3).setPadding(0).setLoop(true);
			animation.setWidth(40).setHeight(44);
		}
	}

	void InitialStats() override {
		maxStates = 5, state = previousState = 0;
		scale = sf::Vector2f(2.5, 2.5);
		speed = 2;
		enemyAI.setRange(500);
		weaponPlug = sf::Vector2f(38 * scale.x, 8 * scale.y);
	}
public:
	BazookaSoldier(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex,aud, x, y) {
		// 0 -> idle, 1 -> running , 2 ->  jump , 3 -> shoot, 4 -> recoil ,
		InitialStats();
		setAnimation(state);
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);
		inventory.addWeapon(new Bazooka(textureManager, audioManager, weaponX, weaponY, direction, false));
	}

	void update() override {
		velocity.x = 0;
		if (isDead()) {
			active = false;
		}
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory.update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}

	void handleInput() override {
		if (isDead()) return;
		bool a = enemyAI.alert(position);
		if (a) {
			MoveAndShoot(0, 1, 2);
		}
		else {
			PatrolState();
		}
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
	void InitialStats() override {
		maxStates = 5, state = previousState = 0;
		scale = sf::Vector2f(2.5, 2.5);
		speed = 5;
	}
public:
	Martian(TextureManager* tex,AudioManager* aud, int x, int y) : Enemy(tex,aud, x, y) {
		// 0 -> idle, 1 -> walk, 2 -> shoot, 3 - >hit, 4 -> die
		InitialStats();
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

class GrenadeSoldier : public Enemy {
private:
	void setAnimation(int state) {

	}
public:

};

class Neil : public Enemy {
	void setAnimation(int state) override {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("neil_idle.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(5).setDelay(500).setPadding(0).setLoop(false);
			animation.setWidth(83).setHeight(46).setReversed(true);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("neil_run.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(100).setPadding(0).setLoop(true);
			animation.setWidth(85).setHeight(46).setReversed(true);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("neil_shoot_one.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(75).setPadding(0).setLoop(true);
			animation.setWidth(128).setHeight(67);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("neil_shoot_two.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(4).setDelay(75).setPadding(0).setLoop(true);
			animation.setWidth(125).setHeight(67);
		}
		else if (state == 4) {
			animation.setTexture(textureManager->getTexture("neil_die.png"));
			animation.setCurrentFrame(0).setStartingFrame(0).setTotalFrames(6).setDelay(300).setPadding(0).setLoop(false);
			animation.setWidth(85).setHeight(46);
		}
	}
	void InitialStats() override {
		maxStates = 5, state = previousState = 0;
		scale = sf::Vector2f(2, 2);
		speed = 1;
		enemyAI.setRange(1000);
		hp = 40;
		weaponPlug = sf::Vector2f(77 * scale.x,31*scale.y);
	}

public:
	Neil(TextureManager* tex, AudioManager* aud, int x, int y) : Enemy(tex, aud, x, y) {
		InitialStats();
		setAnimation(state);
		int weaponX = hitbox.left + (animation.getWidth() * scale.x);
		int weaponY = hitbox.top + (animation.getHeight() * scale.y);
		inventory.addWeapon(new MachineGun(textureManager, audioManager, weaponX + 100, weaponY, direction, false));
	}

	void update() override {
		velocity.x = 0;
		if (isDead()) {
			state = 4;
			if (state == 4 && animation.getDone()) {
				active = false;
			}

		}
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite).cycle();
		hitBoxUpdate();
		float cx = cx = position.x + 0.5f * animation.getWidth() * scale.x;
		cx += (direction == 1) ? weaponPlug.x : -weaponPlug.x;
		inventory.update(sf::Vector2f(cx, position.y + weaponPlug.y), direction);
	}

	void handleInput() override {
		if (isDead()) return;
		bool a = enemyAI.alert(position);
		if (a) {
			MoveAndShoot(0, 1, 2);
		}
		else {
			PatrolState();
		}
	}
};