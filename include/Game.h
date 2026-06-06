#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include "GameStateManager.h";

using namespace std;
using namespace sf;

class Game {
	int screen_x;
	int screen_y;
	RenderWindow window;
	GameStateManager gameStateManager;

public:
	Game();
	void run();
};