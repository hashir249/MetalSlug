#pragma once

#include "PlayState.h"
#include "MenuState.h"
#include "GameState.h"

class GameStateManager {
	GameState** gameStates;
	int stateCount; 	// as for now we are working with two states 1. play state, 2. menu state
	GameState* currentState; // pointing to the proper index in gameStates array
public:
	GameStateManager();
	bool update(RenderWindow& window);
	void setPlayState();
	void setMenuState();
	void startGameMode(int);
	~GameStateManager();
};