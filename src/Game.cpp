#include "Game.h"

Game::Game() : screen(1600, 900), window(VideoMode(screen.x, screen.y), "Metal Slug", Style::Default) {
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