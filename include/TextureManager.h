#pragma once
// the role of this class is to load each texture only once , this whole class would be owned by the main class of GameState and then each subdsequent class would have
// reference to this class

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;


class STRING {
	char* str;
	int len;

	int getLen(const char* str) {
		if (str) {
			int i = 0;
			while (str[i] != '\0') {
				i++;
			}
			return i;
		}

		return 0;
	}

	void strCopy(char*& dest, const char* str) {
		int len = getLen(str);
		dest = new char[len + 1];

		for (int i = 0; i <= len; i++) {
			dest[i] = str[i];
		}

	}

	bool strEqual(const char* first, const char* second) {
		if (first == nullptr || second == nullptr) return false;
		int i = 0;
		while (first[i] != '\0' && second[i] != '\0') {
			if (first[i] != second[i]) {
				return false;
			}
			i++;
		}

		if (first[i] == '\0' && second[i] == first[i]) {
			return true;
		}

		return false;
	}


public:
	STRING(const char* str) {
		strCopy(this->str, str);
	}
	STRING() {
		this->str = nullptr;
		this->len = 0;
	}

	STRING(const STRING& other) {
		this->len = other.len;
		strCopy(this->str, other.str);
	}

	bool operator==(const STRING& other) {
		return (this->len == other.len && strEqual(this->str, other.str));
	}

	STRING& operator=(const STRING& other) {
		if (other != *this) {
			if (str) delete[] str;
			this->len = other.len;
			strCopy(this->str, other.str);
		}

		return *this;
	}

	~STRING() {
		delete[] str;
	}

	bool operator==(const char* str) {
		return strEqual(this->str, str);
	}
};

class TextureManager {
	Texture* textures;
	int capacity;
	int loadedCount;
	STRING* names;

	int getLen(const char* str) const {
		if (str == nullptr) return 0;
		int i = 0;
		while (str[i] != '\0') {
			i++;
		}
		return i;
	}

	const char* join(const char* first, const char* second) {
		int len1 = getLen(first);
		int len2 = getLen(second);
		int len = len1 + len2;
		char* result = new char[len + 1];
		int write = 0;
		for (int i = 0; i < len1; i++) {
			result[write] = first[i];
			write++;
		}
		for (int i = 0; i < len2; i++) {
			result[write] = second[i];
			write++;
		}

		result[write] = '\0';

		return result;
	}
public:
	TextureManager() {
		capacity = 50;
		textures = new Texture[capacity];
		loadedCount = 0;
		names = new STRING[capacity];
	}

	Texture& getTexture(const char* name) {
		for (int i = 0; i < loadedCount; i++) {
			if (names[i] == name) {
				return textures[i];
			}
		}

		if (loadedCount == capacity) {
			resize();
		}
		const char* path = join("../25I-0524_25I-0615_Assets/", name);
		cout << path << endl;
		if (textures[loadedCount].loadFromFile(path)) {
			names[loadedCount] = name;
			loadedCount++;
			cout << "Loaded" << endl;
		}
		cout << "done" << endl;
		delete[] path;

		return textures[loadedCount - 1];
	}

	void resize() {
		Texture* newTextures = new Texture[capacity * 2];
		STRING* newNames = new STRING[capacity * 2];
		for (int i = 0; i < loadedCount; i++) {
			newTextures[i] = textures[i];
			newNames[i] = names[i];
		}

		this->capacity *= 2;

		delete[] textures;
		delete[] names;
		textures = newTextures;
		names = newNames;
	}

	void applyTextureOnSprite(Sprite& s, const char* name) {
		for (int i = 0; i < loadedCount; i++) {
			if (names[i] == name) {
				s.setTexture(textures[i]);
				break;
			}
		}
	}

	~TextureManager() {
		delete[] names;
		delete[] textures;
	}
};