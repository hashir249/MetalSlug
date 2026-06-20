#include "PlayerSoldier.h"

void PlayerSoldier::handleInput() {
	if(Keyboard::isKeyPressed(Keyboard::V)){
		if(mountingTimer.getElapsedTime().asMilliseconds() >= 1000){
			if(insideVehicle()){
				int x, y;
				x = y = 0;
				if(vehicle != nullptr){
					x = vehicle->getX();
					y = vehicle->getY();
					vehicle->unmount();
				}

				leaveVehicle();
				this->setPosition(x - 100, y + 100);

				mountingTimer.restart();
			}
		}
	}

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
		if(mountingTimer.getElapsedTime().asMilliseconds() < 1000) return;
		enterVehicle(v);
		v->mount(this);
		mountingTimer.restart();
	}
}
