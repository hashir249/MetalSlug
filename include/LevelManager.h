// new version
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace sf;
using namespace std;

#include "TextureManager.h"
#include "Block.h"

struct Position {
	int x, y;
	Position(int x = 0, int y = 0) : x(x), y(y) {}
};
struct HitBox {
	int left, top, width, height;
	HitBox(int l = 0, int t = 0, int w = 0, int h = 0) : left(l), top(t), width(w), height(h) {}
};


// blocks for the level
class Level {
protected:
	TextureManager* textureManager;
	Sprite Rock, Sand, Andesite, Deepslate, Grass;
public:
	Level(TextureManager* tex) {
		this->textureManager = tex;
		textureManager->getTexture("sand.png");
		textureManager->getTexture("grass.png");
		textureManager->getTexture("andesite.png");
		textureManager->getTexture("rock.png");
		textureManager->getTexture("deepslate.png");
	}
	virtual void render(RenderWindow&, int, int) = 0;
	virtual void build(int, int) = 0;
	virtual void destroy() = 0;
	virtual bool isBlock(int, int) = 0;
};

// for infinite terrain generation
struct Chunk {
	Block** grid;
	int chunkID;
	Chunk* next;
	Chunk* prev;

	Chunk(int id, int w, int h) : chunkID(id), next(nullptr), prev(nullptr) {
		grid = new Block * [h];
		for (int i = 0; i < h; i++) {
			grid[i] = new Block[w];
		}
	}
	~Chunk() {}
};


class Campaign : public Level {
private:
	Chunk* head;
	Chunk* tail;

	int chunkWidth;
	int levelHeight;
	int cellSize;

	float getHeight(int x) {
		return 20 + sin(x * 0.03) * 4;
	}

	char getBiome(int x) {
		int zone = (x / 120) % 3;

		if (zone == 0) return 'P';
		if (zone == 1) return 'M';
		return 'S';
	}

	char getBlockType(int y, int ground, char biome) {
		if (y == levelHeight - 1) return 'R';

		if (biome == 'P') {
			if (y == ground) return 'G';
			if (y < ground + 4) return 'D';
			return 'R';
		}

		if (biome == 'M') {
			if (y == ground) return 'A';
			if (y < ground + 4) return 'D';
			return 'R';
		}

		if (y == ground) return 'S';
		if (y < ground + 4) return 'D';
		return 'R';
	}

	void generateChunk(Chunk* c) {
		for (int x = 0; x < chunkWidth; x++) {

			int worldX = c->chunkID * chunkWidth + x;

			int ground = getHeight(worldX);
			char biome = getBiome(worldX);

			for (int y = 0; y < levelHeight; y++) {

				if (y >= ground) {
					c->grid[y][x]
						.setFilled(true)
						.setType(getBlockType(y, ground, biome));
				}
				else {
					c->grid[y][x].setFilled(false);
				}
			}
		}
	}

	void appendChunk(int id) {
		Chunk* c = new Chunk(id, chunkWidth, levelHeight);
		generateChunk(c);

		if (!head) {
			head = tail = c;
		}
		else {
			tail->next = c;
			c->prev = tail;
			tail = c;
		}
	}

	void prependChunk(int id) {
		Chunk* c = new Chunk(id, chunkWidth, levelHeight);
		generateChunk(c);

		if (!head) {
			head = tail = c;
		}
		else {
			c->next = head;
			head->prev = c;
			head = c;
		}
	}

	void removeFront() {
		if (!head) return;

		Chunk* temp = head;
		head = head->next;

		if (head) head->prev = nullptr;

		for (int i = 0; i < levelHeight; i++)
			delete[] temp->grid[i];

		delete[] temp->grid;
		delete temp;
	}

	void removeBack() {
		if (!tail) return;

		Chunk* temp = tail;
		tail = tail->prev;

		if (tail) tail->next = nullptr;

		for (int i = 0; i < levelHeight; i++)
			delete[] temp->grid[i];

		delete[] temp->grid;
		delete temp;
	}

public:
	Campaign(TextureManager* tex)
		: Level(tex),
		head(nullptr),
		tail(nullptr),
		chunkWidth(60),
		levelHeight(36),
		cellSize(64) {

		textureManager->applyTextureOnSprite(Grass, "grass.png");
		textureManager->applyTextureOnSprite(Andesite, "andesite.png");
		textureManager->applyTextureOnSprite(Sand, "sand.png");
		textureManager->applyTextureOnSprite(Deepslate, "deepslate.png");
		textureManager->applyTextureOnSprite(Rock, "rock.png");

		appendChunk(-1);
		appendChunk(0);
		appendChunk(1);
	}

	bool isBlock(int x, int y) override {

		int chunkID = x / (chunkWidth * cellSize);

		int localX = (x / cellSize) % chunkWidth;
		int localY = y / cellSize;

		Chunk* curr = head;

		while (curr) {
			if (curr->chunkID == chunkID)
				return curr->grid[localY][localX].isFilled();

			curr = curr->next;
		}

		return false;
	}

	void build(int, int) override {}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {

		int leftChunk = scroll_x / (chunkWidth * cellSize);

		if (head->chunkID < leftChunk - 1) {
			removeFront();
			appendChunk(tail->chunkID + 1);
		}

		if (head->chunkID > leftChunk - 1) {
			removeBack();
			prependChunk(head->chunkID - 1);
		}

		Chunk* curr = head;

		while (curr) {

			for (int y = 0; y < levelHeight; y++) {
				for (int x = 0; x < chunkWidth; x++) {

					if (!curr->grid[y][x].isFilled())
						continue;

					int worldX = (curr->chunkID * chunkWidth + x) * cellSize;
					int worldY = y * cellSize;

					char t = curr->grid[y][x].getType();

					Sprite* s = &Grass;

					if (t == 'A') s = &Andesite;
					else if (t == 'S') s = &Sand;
					else if (t == 'D') s = &Deepslate;
					else if (t == 'R') s = &Rock;

					s->setPosition(worldX - scroll_x, worldY - scroll_y);
					window.draw(*s);
				}
			}

			curr = curr->next;
		}
	}

	void destroy() override {
		while (head) removeFront();
	}

	~Campaign() {
		destroy();
	}
};

// survival level
class Survival : public Level {
private:
	int currentLevel;
	int levelWidth;
	int levelHeight;
	int cellSize;
	Block** map;

public:
	Survival(TextureManager* tex, int width, int height, int cellSz) : Level(tex), levelWidth(width), levelHeight(height), cellSize(cellSz) {
		build(levelWidth, levelHeight);
		currentLevel = 1;
		setupLevel(currentLevel);
	}

	void build(int width, int height) override {
		map = new Block * [height];
		for (int i = 0; i < height; i++) {
			map[i] = new Block[width];
			for (int j = 0; j < width; j++)
				map[i][j].setType('\0');
		}
	}

	bool isBlock(int x, int y) override {
		if (x < 0 || x > cellSize * levelWidth || y < 0 || y > cellSize * levelHeight)
			return true;
		if ((y / cellSize) < 0 || (y / cellSize) >= levelHeight - 1)
			return true;
		if ((x / cellSize) < 0 || (x / cellSize) >= levelWidth - 1)
			return true;
		char t = map[y / cellSize][x / cellSize].getType();
		return (t == 'R' || t == 'A' || t == 'D' || t == 'G' || t == 'S');
	}

	void setupLevel(int level) {
		for (int j = 0; j < levelWidth; j++) {
			map[0][j].setType('D');
			map[levelHeight - 1][j].setType('R');
		}

		for (int i = 2; i < 13; i++) {
			for (int j = 0; j < levelWidth; j++) {
				if (i == 12) {
					map[levelHeight - i][j].setType('G');
				}
				else {
					map[levelHeight - i][j].setType('S');
				}
			}
		}

		if (level == 1) {

		}
	}

	void render(RenderWindow& window, int scroll_x, int scroll_y) override {
		for (int i = 0; i < levelHeight; i++) {
			for (int j = 0; j < levelWidth; j++) {
				char t = map[i][j].getType();
				if (t == 'R') {
					textureManager->applyTextureOnSprite(Rock, "rock.png");
					Rock.setPosition(j * cellSize - scroll_x, i * cellSize - scroll_y);
					window.draw(Rock);
				}
				else if (t == 'S') {
					textureManager->applyTextureOnSprite(Sand, "sand.png");
					Sand.setPosition(j * cellSize - scroll_x, i * cellSize - scroll_y);
					window.draw(Sand);
				}
				else if (t == 'D') {
					textureManager->applyTextureOnSprite(Deepslate, "deepslate.png");
					Deepslate.setPosition(j * cellSize - scroll_x, i * cellSize - scroll_y);
					window.draw(Deepslate);
				}
				else if (t == 'G') {
					textureManager->applyTextureOnSprite(Grass, "grass.png");
					Grass.setPosition(j * cellSize - scroll_x, i * cellSize - scroll_y);
					window.draw(Grass);
				}
				else if (t == 'A') {
					textureManager->applyTextureOnSprite(Andesite, "andesite.png");
					Andesite.setPosition(j * cellSize - scroll_x, i * cellSize - scroll_y);
					window.draw(Andesite);
				}
			}
		}
	}

	void destroy() override {
		if (map) {
			for (int i = 0; i < levelHeight; i++)
				delete[] map[i];
			delete[] map;
		}
	}

	~Survival() {
		destroy();
	}
};

class LevelManager {
	TextureManager* textureManager;
	Level* level;
public:
	LevelManager() : level(nullptr), textureManager(nullptr) {}
	LevelManager(TextureManager* tex) : textureManager(tex), level(nullptr) {}

	void setTexture(TextureManager* tex) {
		this->textureManager = tex;
	}

	void setupSurvivalMode() {
		level = new Survival(textureManager, 180, 36, 64);
	}
	void setupCampaignMode() {
		level = new Campaign(textureManager);
	}
	void render(RenderWindow& window, int scroll_x, int scroll_y) {
		if (level) level->render(window, scroll_x, scroll_y);
	}
	void update() {}
	Level* getLevel() {
		return level;
	}
};