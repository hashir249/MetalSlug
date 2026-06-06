#pragma once

#include "GameState.h"

class MenuState : public GameState {
	Texture menuTex;
	Sprite menuSprite;
public:
	MenuState(GameStateManager* manager);
	int handleInput() override;
	void render(RenderWindow& window) override;
	void update() override;
};