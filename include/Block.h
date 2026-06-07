#pragma once

class Block {
	char type; // 'D' for deepslate, 'G' for grass, 'A' for andesite, 'R' for rock , 'S' for sand
	int hp;
	bool filled;
	int state;
public:
	Block(char type = '\0', int hp = 0, bool filled = 0, int state = 0) : type(type), hp(hp), filled(filled), state(state) {}

	// getters
	int getHP() const {
		return hp;
	}
	char getType() const {
		return type;
	}
	bool isFilled() const {
		return filled;
	}
	int getState() const {
		return state;
	}

	// setters 
	Block& setHP(int hp) {
		this->hp = hp;
		return *this;
	}
	Block& setType(char type) {
		if (type == 'D' || type == 'S' || type == 'G' || type == 'A' || type == 'R')
			this->type = type;

		return *this;
	}
	Block& setFilled(bool filled) {
		this->filled = filled;
		return *this;
	}
	Block& setState(int state) {
		this->state = state;
		return *this;
	}

	void destroy() {
		type = '\0';
		hp = 0;
		filled = false;
		state = 0;
	}
};
