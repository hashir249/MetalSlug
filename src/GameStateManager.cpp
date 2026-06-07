#include "GameStateManager.h"

GameStateManager::GameStateManager() : stateCount(2) {

	gameStates = new GameState * [stateCount]; // as the game starts teh first state would be by default the menu state to choose options
	gameStates[0] = new MenuState(this);
	gameStates[1] = new PlayState(this);

	// current state would always start with menu displaying
	currentState = gameStates[0];
}

bool GameStateManager::update(RenderWindow& window) {
	int response = currentState->handleInput();
	if (response == 0) {
		currentState->update();
		currentState->render(window);
	}
	else {
		if (response == 2) {
			startGameMode(1);
		}
		else if (response == 3) {
			startGameMode(2);
		}
		else if (response == 5 || response == 4 || response == 1) {
			return false;
		}
	}

	return true;
}

void GameStateManager::setPlayState() {
	currentState = gameStates[1];
}

void GameStateManager::setMenuState() {
	currentState = gameStates[0];
}

GameStateManager::~GameStateManager() {
	for (int i = 0; i < stateCount; i++) {
		delete gameStates[i];
	}

	delete[] gameStates;
}

void GameStateManager::startGameMode(int mode) {
	this->setPlayState();
	currentState->setup(mode);
}