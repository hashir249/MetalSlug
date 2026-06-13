#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class AnimationManager {
	Texture* tex;
	int currentFrame;
	int totalFrames;
	Clock frameTimer;
	int width, height;
	int delay;
	bool loop;
	bool done;
	int padding;
	int startingFrame;
	bool reversed;
	bool forwardPass;
public:
	AnimationManager() {
		tex = nullptr;
		width = height = 0;
		currentFrame = totalFrames = delay = 0;
		padding = 0;
		reversed = false;
		loop = true;
		done = false;
		forwardPass = true;
	}

	AnimationManager& apply(Sprite& sprite) {
		if (tex == nullptr) return *this;
		sprite.setTexture(*tex);
		sprite.setTextureRect(IntRect((currentFrame)*width + ((currentFrame != 0) ? currentFrame * padding : 0), 0, width, height)); // left, top, width, height
		return *this;
	}

	AnimationManager& setTexture(Texture& tex) {
		this->tex = &tex;
		return *this;
	}
	AnimationManager& setReversed(bool val) {
		this->reversed = val;
		return *this;
	}
	AnimationManager& setCurrentFrame(int frame) {
		this->currentFrame = frame;
		done = false;
		return *this;
	}
	AnimationManager& setPadding(int padding) {
		this->padding = padding;
		return *this;
	}

	AnimationManager& setTotalFrames(int count) {
		this->totalFrames = count;
		return *this;
	}

	AnimationManager& setDelay(int delay) {
		this->delay = delay;
		return *this;
	}

	AnimationManager& setLoop(bool option) {
		loop = option;
		return *this;
	}

	AnimationManager& setStartingFrame(int f) {
		this->startingFrame = f;
		return *this;
	}
	AnimationManager& setWidth(int width) {
		this->width = (width < 0) ? 0 : width;
		return *this;
	}

	AnimationManager& setHeight(int height) {
		this->height = (height < 0) ? 0 : height;
		return *this;
	}


	bool getDone() const {
		return (loop == false && done == true);
	}

	void cycle() {
		if (frameTimer.getElapsedTime().asMilliseconds() > delay) {
			if (reversed) {
				if (loop) {
					if (forwardPass) {
						currentFrame++;
						if (currentFrame >= totalFrames - 1) {
							currentFrame = totalFrames - 1;
							forwardPass = false;
						}
					}
					else {
						currentFrame--;
						if (currentFrame <= 0) {
							currentFrame = 0;
							forwardPass = true;
						}
					}
				}
				else {
					if (forwardPass) {
						currentFrame++;
						if (currentFrame >= totalFrames - 1) {
							currentFrame = totalFrames - 1;
							forwardPass = false;
						}
					}
					else {
						currentFrame--;
						if (currentFrame <= 0) {
							currentFrame = 0;
							done = true;
						}
					}
				}
			}
			else {
				currentFrame++;
				if (currentFrame >= totalFrames) {
					if (loop) {
						currentFrame = startingFrame;
					}
					else {
						currentFrame = totalFrames - 1;
						done = true;
					}
				}
			}
			frameTimer.restart();
		}
	}

	AnimationManager& setDone(bool option) {
		done = option;
		return *this;
	}
	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}
};