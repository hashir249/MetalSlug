#include "PlayState.h"


PlayState::PlayState(GameStateManager* manager) : GameState(manager), entityManager(&textureManager, &collisionManager, 1), camera_x(0), camera_y(0) {
	survivalCode = 1;
	campaignCode = 2;
	levelManager.setTexture(&textureManager);
	collisionManager.setLevel(levelManager.getLevel());
}

void PlayState::setCameraX(int val) {
	this->camera_x = val;
	camera_x = (camera_x < 0) ? 0 : camera_x;
}

void PlayState::setCameraY(int val) {
	this->camera_y = val;
	camera_y = (camera_y < 0) ? 0 : camera_y;
}

int PlayState::handleInput() {
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		setCameraY(camera_y - 20);
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		setCameraX(camera_x - 20);
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		setCameraY(camera_y + 20);
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		setCameraX(camera_x + 20);
	}
	return 0;
}

void PlayState::render(RenderWindow& window) {
	levelManager.render(window, camera_x, camera_y);
	//entityManager.render(window, camera_x, camera_y);
}

void PlayState::update() {
	levelManager.update();
	//entityManager.update();
	//collisionManager.setLevel(levelManager.getLevel());
}

void PlayState::setup(int choice) {
	if (choice == survivalCode) {
		levelManager.setupSurvivalMode();
	}
	else if (choice == campaignCode) {
		levelManager.setupCampaignMode();
	}
}