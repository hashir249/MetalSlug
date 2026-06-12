#pragma once
#include <string>
#include "SFML/Audio.hpp"
using namespace std;
using namespace sf;

class AudioManager {
	sf::SoundBuffer* buffers;
	int capacity;
	int loadedCount;
	string* names;

	void resize() {
		sf::SoundBuffer* newTextures = new sf::SoundBuffer[capacity * 2];
		string* newNames = new string[capacity * 2];
		for (int i = 0; i < loadedCount; i++) {
			newTextures[i] = buffers[i];
			newNames[i] = names[i];
		}

		this->capacity *= 2;

		delete[] buffers;
		delete[] names;
		buffers = newTextures;
		names = newNames;
	}
public:
	AudioManager() {
		capacity = 50;
		buffers = new sf::SoundBuffer[capacity];
		loadedCount = 0;
		names = new string[capacity];
	}

	sf::SoundBuffer& getBuffer(string name) {
		for (int i = 0; i < loadedCount; i++) {
			if (names[i] == name) {
				return buffers[i];
			}
		}

		if (loadedCount == capacity) {
			//resize();
		}
		string path = "../../assets/audios/" + name;
		cout << path << endl;
		if (buffers[loadedCount].loadFromFile(path)) {
			names[loadedCount] = name;
			loadedCount++;
			cout << "Loaded" << endl;
		}
		cout << "done" << endl;
		

		return buffers[loadedCount - 1];
	}

	~AudioManager() {
		delete[] names;
		delete[] buffers;
	}
};