#include "PlayerSoldier.h"

void PlayerSoldier::handleInput() {
    if (insideVehicle()) vehicle->handleInput();
		else {
			velocity.x = 0;
			moving = false;
			if (state == 4 || state == 5) {
				if (animation.getDone()) state = 0;
				//return;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				velocity.x = speed;
				direction = 1;
				moving = true;
				if (onGround) state = 1;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				velocity.x = -speed;
				direction = 2;
				moving = true;
				if (onGround) state = 1;
			}
			else if (onGround) state = 0;

			if (Keyboard::isKeyPressed(Keyboard::Up) && onGround) {
				velocity.y = -jumpForce;
				onGround = false;
				state = 2;
			}
			if (Keyboard::isKeyPressed(Keyboard::M)) {
				state = 4;
				return;
			}
			if (Keyboard::isKeyPressed(Keyboard::G)) {
				state = 5;
				product = throwGrenade();

			}
			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				state = 3;
				product = inventory->fire();
			}
			if (Keyboard::isKeyPressed(Keyboard::X)) {
				inventory->nextWeapon();
			}
		}
}

void PlayerSoldier::interactWithVehicle(Vehicle* v) {
	if(Keyboard::isKeyPressed(sf::Keyboard::V)){
		if(insideVehicle()) {
			leaveVehicle();
			v->unmount();
			int x = v->getX();
			int y = v->getY();

			this->setPosition(x - 100, y + 100);
		}
		else{
			enterVehicle(v);
			v->mount(this);
		}
	}
}
