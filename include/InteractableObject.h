#pragma once

#include "Entity.h"

class Player;

class InteractableObject : public Entity {
protected:
	bool interacted;
public:
	InteractableObject(TextureManager* t, AudioManager* aud,int x, int y) : Entity(t,aud, x, y) {
		interacted = false;
		active = true;
		state = 0;
	}
	bool hasBeenInteracted() const {
		return interacted;
	}
};

