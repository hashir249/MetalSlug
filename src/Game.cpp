#include "Game.h"

Game::Game() : screen_x(1600), screen_y(900), window(VideoMode(screen_x, screen_y), "Metal Slug", Style::Default) {
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
}

void Game::run() {
	Event ev;
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}
			if (ev.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					window.close();
				}
			}
		}
		window.clear();
		bool response = gameStateManager.update(window);
		if (response == false)
			window.close();
		window.display();
	}
}