#pragma once

#include "Entity.h"

class Collectible : public Entity {
protected:

public:
	Collectible(TextureManager* tex, AudioManager* aud, int x, int y) : Entity(tex, aud, x, y) {

	}
};

class Fruit : public Collectible {

public:
	
};

class Turkey : public Collectible {

};

class SupplyCrate : public Collectible {

};