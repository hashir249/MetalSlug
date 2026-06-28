#pragma once

#include "SFML/Graphics.hpp"
using namespace sf;

class EnemyAIState {
	int range{}; // range in which it should scan the player and attack it
	int buffer{};
	int distance{};
	sf::Vector2f target{};
	sf::Vector2f projectile{};
	int patrolDirection = 1;     // 1 right, 2 left
	int patrolDistance = 0;
	int patrolLimit = 0;
public:
	EnemyAIState() {
		
	}
	void setRange(int r, bool applyBuffer = true) {
		range = r;
		if (applyBuffer) buffer = 8.f * r / 10.f;
		else buffer = 0;
		patrolLimit = 3.f * range / 10.f;
	}

	bool alert(sf::Vector2f pos) {
		sf::Vector2f diff = pos - target;
		bool movement = false;
		int absDiffX = (diff.x > 0) ? diff.x : -diff.x;

		bool alert = false;
		if (absDiffX <= range) alert = true;
		return alert;
	}

	int directionSet(sf::Vector2f pos) {
		int direction = 0;
		if (target.x > pos.x) {
			direction = 1;
		}
		else direction = 2;

		return direction;
	}

	bool applyMovement(sf::Vector2f pos) {
		sf::Vector2f diff = pos - target;
		bool movement = false;
		int absDiffX = (diff.x > 0) ? diff.x : -diff.x;
		
		int appliedBuffer = buffer + ((rand() % 2) ? 1 : -1) * (2 / 10 * buffer);
		if (absDiffX <= range && absDiffX >= appliedBuffer) {
			movement = true;
		}

		return movement;
	}


	bool applyShooting(sf::Vector2f pos) {
		sf::Vector2f diff = pos - target;
		bool movement = false;
		int absDiffX = (diff.x > 0) ? diff.x : -diff.x;
		bool shoot = false;
		if (absDiffX <= range) shoot = true;

		return shoot;
	}

	void addDistance(int d) {
		this->distance += d;
	}

	void patrol(int speed)
	{
		patrolDistance += speed;

		if (patrolDistance >= patrolLimit)
		{
			patrolDistance = 0;

			if (patrolDirection == 1)
				patrolDirection = 2;
			else
				patrolDirection = 1;
		}
	}

	int getPatrolDirection()
	{
		return patrolDirection;
	}

	void updateTarget(sf::Vector2f pos) {
		this->target = pos;
	}
	void updateProjectile(sf::Vector2f pos) {
		this->projectile = pos;
	}
};