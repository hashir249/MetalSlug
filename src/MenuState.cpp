#include "MenuState.h"

MenuState::MenuState(GameStateManager* manager) : GameState(manager) {
	menuTex.loadFromFile("../../assets/textures/main_menu.png");
	menuSprite.setTexture(menuTex);
	menuSprite.setPosition(0, 0);
}

int MenuState::handleInput() {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		return 1;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		return 2;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
		return 3;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num4)) {
		return 4;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num5)) {
		return 5;
	}

	return 0;
}

void MenuState::render(RenderWindow& window) {
	window.draw(menuSprite);
}

void MenuState::update() {

}