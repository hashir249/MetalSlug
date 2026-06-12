#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "TextureManager.h"
#include "AudioManager.h"

class GameStateManager;

using namespace std;
using namespace sf;

class GameState {
protected:
	TextureManager textureManager;
	AudioManager audioManager;
	GameStateManager* gameStateManager; // does not own it , but to bring changes it will bring about changes 
	// menu or play states will tell the manager to transition states by calling functions inside the manager
public:
	GameState(GameStateManager*);
	virtual int handleInput() = 0;
	virtual void render(RenderWindow&) = 0;
	virtual void update() = 0;
	virtual void setup(int);
};