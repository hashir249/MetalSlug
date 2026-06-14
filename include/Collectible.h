#pragma once

#include "Entity.h"
#include "PlayerSoldier.h"

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

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		sprite.setScale(scale.x * (direction == 1 ? 1 : -1), scale.y); // if direction = 1 that is right in our case, if not then flipping the x-axis
		sprite.setPosition(position.x - scroll_x + 0.5 * (animation.getWidth() * scale.x), position.y - scroll_y);
		window.draw(sprite);
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
		animation.setTotalFrames(11).setCurrentFrame(0).setDelay(250).setLoop(true);
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

	void update() override {
		animation.apply(sprite).cycle();
		hitBoxUpdate();
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

	void update() override {
		animation.apply(sprite).cycle();
		hitBoxUpdate();
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
		animation.apply(sprite).cycle();
		hitBoxUpdate();
	}

	void interactWithPlayer(PlayerSoldier* p) override {
		if (Keyboard::isKeyPressed(sf::Keyboard::E)) {
			Inventory* i = p->getInventory();
			int weaponCount = i->getWeaponCount();
			weaponCount = (weaponCount < 1) ? 1 : weaponCount;
			srand(time(0));

			int choice = rand() % weaponCount;

			i->addAmmo(choice, (rand() % 5) * 10);
			
			choice = rand() % 100 + 1;
			if (choice <= 90) {
				int c = rand() % 3;
				if (c == 0) {
					if (i->getWeapon("ShotGun") != nullptr) {
						Weapon* w = i->getWeapon("ShotGun");
						if (w != nullptr) w->addAmmo(20);
					}
					else {
						i->addWeapon(new ShotGun(textureManager, audioManager, position.x, position.y, direction));
						i->setWeapon("ShotGun");
					}
				}
				else if (c == 1) {
					if (i->getWeapon("MachineGun") != nullptr) {
						Weapon* w = i->getWeapon("MachineGun");
						if (w != nullptr) w->addAmmo(20);
					}
					else {
						i->addWeapon(new MachineGun(textureManager, audioManager, position.x, position.y, direction));
						i->setWeapon("MachineGun");
					}
				}
				else if (c == 2) {
					if (i->getWeapon("Bazooka") != nullptr) {
						Weapon* w = i->getWeapon("Bazooka");
						if (w != nullptr) w->addAmmo(20);
					}
					else {
						i->addWeapon(new Bazooka(textureManager, audioManager, position.x, position.y, direction));
						i->setWeapon("Bazooka");
					}
				}
			}
			else {
				if (i->getWeapon("LaserGun") != nullptr) {
					Weapon* w = i->getWeapon("LaserGun");
					if (w != nullptr) w->addAmmo(20);
				}
				else {
					i->addWeapon(new LaserGun(textureManager, audioManager, position.x, position.y, direction));
					i->addAmmo(weaponCount, 20);
					i->setWeapon("LaserGun");
				}
			}
			active = false;
		}
	}

	~SupplyCrate() {

	}
};


class PowPrisoner : public Collectible {
private:
	bool freed;
	float walkSpeed;
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("prisoner_chained_idle.png"));
			animation.setTotalFrames(3).setCurrentFrame(0).setDelay(750).setLoop(true).setStartingFrame(0);
			animation.setWidth(32).setHeight(27);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("prisoner_freed.png"));
			animation.setTotalFrames(2).setCurrentFrame(0).setDelay(1000).setLoop(true).setStartingFrame(0);
			animation.setWidth(40).setHeight(31);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("prisoner_jogging.png"));
			animation.setTotalFrames(3).setCurrentFrame(0).setDelay(300).setLoop(true).setStartingFrame(0);
			animation.setWidth(22).setHeight(39);
		}
	}
public:
	PowPrisoner(TextureManager* t, AudioManager* aud, int x, int y) : Collectible(t, aud, x, y) {
		freed = false;
		walkSpeed = 5.f;
		direction = 1;
		maxStates = 3;
		scale.x = 3;
		scale.y = 3;
		previousState = 0;
		setAnimation(0);
	}

	void update() override {
		handleInput();
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();
		if (this->state == 2) {
			velocity.x = (direction == 1 ? -1 : 1) * walkSpeed;
		}
		if (position.x < 10) {
			active = false;
			return;
		}
		if (this->state == 2 && animation.getDone()) {
			state = 3;
		}
		hitBoxUpdate();
	}

	void interactWithPlayer(PlayerSoldier* p) override {
		if (Keyboard::isKeyPressed(sf::Keyboard::E) && !freed) {
			product = new SupplyCrate(textureManager, audioManager, position.x - 35, position.y - 35);
			freed = true;
			state = 2;
		}
	}

	~PowPrisoner() {

	}
};

class Hen : public Collectible {
	void setAnimation(int state) {
		if (state == 0) {
			animation.setTexture(textureManager->getTexture("hen_first.png"));
			animation.setTotalFrames(47).setCurrentFrame(0).setDelay(250).setLoop(false).setStartingFrame(0);
			animation.setWidth(9).setHeight(17);
		}
		else if (state == 1) {
			animation.setTexture(textureManager->getTexture("hen_second.png"));
			animation.setTotalFrames(3).setCurrentFrame(0).setDelay(250).setLoop(false).setStartingFrame(0);
			animation.setWidth(11).setHeight(12);
		}
		else if (state == 2) {
			animation.setTexture(textureManager->getTexture("hen_third.png"));
			animation.setTotalFrames(4).setCurrentFrame(0).setDelay(250).setLoop(false).setStartingFrame(0);
			animation.setWidth(17).setHeight(23);
		}
		else if (state == 3) {
			animation.setTexture(textureManager->getTexture("hen_fourth.png"));
			animation.setTotalFrames(14).setCurrentFrame(0).setDelay(250).setLoop(false).setStartingFrame(0);
			animation.setWidth(21).setHeight(27);
		}
	}
public:
	Hen(TextureManager* tex, AudioManager* aud, int x, int y) : Collectible(tex, aud, x, y) {
		maxStates = 4; // only being displayed -> there is no other state for it
		state = 0;
		velocity = sf::Vector2f(0, 0);
		scale.x = scale.y = 3;
		onGround = true;
		direction = 2; 
		setAnimation(0);
	}

	void update() override {
		handleInput();
		if (animation.getDone()) {
			state++;
			if (state == maxStates) active = false;
		}
		if (state != previousState) {
			setAnimation(state);
			previousState = state;
		}
		animation.apply(sprite);
		animation.cycle();
	
		hitBoxUpdate();
	}



	~Hen() {

	}
};
