
#include "StraightProjectile.h"
#include "Enemy.h"
#include "PlayerSoldier.h"

void Bullet::interactWithPlayer(PlayerSoldier* p)  {
    p->interactWithProjectile(this);
}

void Bullet::interactWithEnemy(Enemy* p) {
    p->interactWithProjectile(this);
}