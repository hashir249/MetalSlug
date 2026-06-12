#pragma once

#include "GameState.h"
#include "LevelManager.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"

class PlayState : public GameState {
	int survivalCode, campaignCode;
	int camera_x;
	int camera_y;
	LevelManager levelManager;
	EntityManager entityManager;
	CollisionManager collisionManager;
public:
	PlayState(GameStateManager*);
	int handleInput() override;

	void render(RenderWindow&) override;
	void update() override;
	void setup(int) override;
	void setCameraX(int);
	void setCameraY(int);
};