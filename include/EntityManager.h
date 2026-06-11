#pragma once

#include "PlayerSoldier.h"
#include "CollisionManager.h"

class EntityManager {
	Entity** players;
	int maxPlayers;
	int playersCount;
	int currentPlayer;

	Entity** entities;
	int entityCount;
	int maxEntities;
	int currentLevel;
	TextureManager* textureManager;
	CollisionManager* collisionManager;

	void resize(int newSize) {
		if (newSize > maxEntities) {
			maxEntities = newSize;
			Entity** newEntities = new Entity * [newSize];
			for (int i = 0; i < entityCount; i++) {
				newEntities[i] = entities[i];
			}
			int i = entityCount;
			for (; i < maxEntities; i++) {
				newEntities[i] = nullptr;
			}

			delete[] entities;
			entities = newEntities;
		}
	}

	void cleanup() {
		if (entities) {
			for (int i = 0; i < entityCount; i++) {
				if (entities[i]) delete entities[i];
			}

			delete[] entities;
		}
	}
	void maintainActive() {
		int idx = 0;
		for (int i = 0; i < entityCount; i++) {
			if (!entities[i]->getActive()) {
				idx = i;
				delete entities[i];
				entities[i] = nullptr;

				for (int j = idx + 1; j < entityCount; j++) {
					entities[j - 1] = entities[j];
				}
				entityCount--;
				entities[entityCount] = nullptr;
			}
		}
	}
	static bool entityOverlap(const sf::IntRect& first, const sf::IntRect& second) {
		return !(first.left + first.width <= second.left ||
			second.left + second.width <= first.left ||
			first.top + first.height <= second.top ||
			second.top + second.height <= first.top);
	}


	void checkPlayerInteractions() {
		sf::IntRect playerBox = getCurrentPlayer()->getHitBox();
		for (int i = 0; i < entityCount; i++) {
			if (entityOverlap(playerBox, entities[i]->getHitBox())) {
				getCurrentPlayer()->interact(entities[i]);
			}
		}
	}

	void addPlayer(Entity* p) {
		if (playersCount == maxPlayers) return;
		players[playersCount] = p;
		playersCount++;
	}

	void nextPlayer() {
		sf::Vector2f p = getCurrentPlayer()->getPosition();
		int direction = getCurrentPlayer()->getDirection();
		currentPlayer = (currentPlayer + 1) % playersCount;
		getCurrentPlayer()->setPosition(p);
		getCurrentPlayer()->setDirection(direction);
	}

public:
	EntityManager(TextureManager* tex, CollisionManager* c, int level) : textureManager(tex), collisionManager(c), currentLevel(level) {
		maxEntities = 20;
		entities = new Entity * [maxEntities];
		for (int i = 0; i < 20; i++) {
			entities[i] = nullptr;
		}
		entityCount = 0;
		setupLevel(currentLevel);

		// players are maintained separately
		maxPlayers = 5;
		players = new Entity * [maxPlayers];
		playersCount = 0;

		addPlayer(new Fiolina(textureManager, 200, 1300));
		addPlayer(new Tarma(textureManager, 300, 1300));
		addPlayer(new Marco(textureManager, 500, 1300));
		addPlayer(new Eri(textureManager, 400, 1300));
	}

	void setupLevel(int level) {
		if (level == 1) {
			
		}
	}

	void update() {
		handleInput();

		//Entity* currentPlayer = getCurrentPlayer();
		//if (ps->inVehicle()) {
		//	getCurrentPlayer()->setHide(true);
		//}
		//else {
		//	currentPlayer->update();
		//	currentPlayer->handleInput();
		//	collisionManager->resolve(currentPlayer);
		//	Entity* shot = currentPlayer->productEntity();
		//	if (shot) addEntity(shot);
		//}

		//for (int i = 0; i < entityCount; i++) {
		//	entities[i]->handleInput();
		//	entities[i]->update();
		//	collisionManager->resolve(entities[i]); // for collision

		//	Entity* created = entities[i]->productEntity();
		//	if (created) addEntity(created);
		//	if (created) {
		//		cout << "ts is working" << endl;
		//	}
		//}

		//// checking interactions betweeen differntn entites
		//checkPlayerInteractions();
		//checkProjectileEnemyCollisions();

		//maintainActive();
		getCurrentPlayer()->update();
		collisionManager->resolve(getCurrentPlayer());
	}

	Entity* getCurrentPlayer() {
		return players[currentPlayer];
	}

	void handleInput() {
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			nextPlayer();
		}
	}

	void addEntity(Entity* e) {
		if (entityCount == maxEntities) {
			resize(this->maxEntities + 20);
		}
		entities[entityCount] = e;
		entityCount++;
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		getCurrentPlayer()->render(window, scroll_x, scroll_y);
		for (int i = 0; i < entityCount; i++) {
			entities[i]->render(window, scroll_x, scroll_y);
		}
	}

	~EntityManager() {
		cleanup();
	}
};