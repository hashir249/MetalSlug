#pragma once

#include "Entity.h"
#include "PlayerSoldier.h"
#include "CollisionManager.h"
#include "Projectile.h"
#include "Collectible.h"
#include "InteractableObject.h"
#include "Enemy.h"
#include "Vehicle.h"

class EntityManager {
	Entity** players;
	int maxPlayers;
	int playersCount;
	int currentPlayer;

	Entity** entities;
	int entityCount;
	int maxEntities;
	int currentLevel;
	AudioManager* audioManager;
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

	void checkProjectileCollisions() {
		for (int i = 0; i < entityCount; i++) {
			if (!entities[i]->getProjectileStatus()) continue;
			for (int j = 0; j < entityCount; j++) {
				if (i == j || entities[j]->getProjectileStatus()) continue;
				if (!entities[j]->getEnemyStatus()) continue;
				if (entityOverlap(entities[i]->getHitBox(), entities[j]->getHitBox())) {
					entities[i]->interact(entities[j]);
				}
			}
		}
	}

	void nextPlayer() {
		sf::Vector2f p = getCurrentPlayer()->getPosition();
		int direction = getCurrentPlayer()->getDirection();
		currentPlayer = (currentPlayer + 1) % playersCount;
		getCurrentPlayer()->setPosition(p);
		getCurrentPlayer()->setDirection(direction);
	}

public:
	EntityManager(TextureManager* tex, CollisionManager* c,AudioManager* aud, int level) : textureManager(tex), collisionManager(c), audioManager(aud), currentLevel(level) {
		maxEntities = 20;
		entities = new Entity * [maxEntities];
		for (int i = 0; i < 20; i++) {
			entities[i] = nullptr;
		}
		entityCount = 0;
		// players are maintained separately
		maxPlayers = 5;
		players = new Entity * [maxPlayers];
		playersCount = 0;
			
		addPlayer(new Fiolina(textureManager,audioManager, 200, 1300));
		addPlayer(new Marco(textureManager,audioManager, 500, 1300));
		addPlayer(new Eri(textureManager,audioManager, 400, 1300));
		addPlayer(new Tarma(textureManager,audioManager, 300, 100));
		//addEntity(new PowPrisoner(textureManager, audioManager, 600, 1200));
		
		for (int i = 0; i < 1; i++) {
			//addEntity(new PatrolRobot(textureManager, audioManager, 1000 + i * 100, 1300));
			addEntity(new ShieldedSoldier(textureManager, audioManager, 2800 + i * 100, 1200));
			addEntity(new Zombie(textureManager, audioManager, 4000 + i * 200, 1200));
			addEntity(new Neil(textureManager, audioManager, 7000 + i * 300, 1300));
			addEntity(new MummyWarrior(textureManager, audioManager, 9000 + i * 100, 1300));
			addEntity(new BazookaSoldier(textureManager, audioManager, 10000 + i * 200, 1400));
		}

		for (int i = 0; i < 1; i++) {
			addEntity(new MetalSlug(textureManager, audioManager, 600, 1200));
		}
	}

	void update() {
		handleInput();
		for (int i = 0; i < entityCount; i++) {
			if (entities[i]->getEnemyStatus()) entities[i]->setTarget(getCurrentPlayer()->getPosition());
			if (entities[i]->getProjectileStatus()) entities[i]->setTarget(getCurrentPlayer()->getPosition());
			//entities[i]->handleInput();
			entities[i]->update();
			collisionManager->resolve(entities[i]); // for collision

			Entity* created = entities[i]->productEntity();
			if (created) addEntity(created);
			if (created) {
				
			}
		}
		checkProjectileCollisions();
		checkPlayerInteractions();
		maintainActive();
		getCurrentPlayer()->update();
		Entity* shot = getCurrentPlayer()->productEntity();
		if (shot) addEntity(shot);
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
			if(entities[i]) entities[i]->render(window, scroll_x, scroll_y);
		}
	}

	~EntityManager() {
		cleanup();
	}
};