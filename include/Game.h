#pragma once
#include "SFML/Graphics.hpp"
#include "GameStateManager.h"
using namespace sf;

class Game {
	const Vector2f screen;
	RenderWindow window;
	GameStateManager gameStateManager;
public:
	Game();
	void run();
};