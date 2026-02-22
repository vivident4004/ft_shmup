#include "includes/enemy.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"
#include "includes/utilities.hpp"

Enemy::Enemy(int x, int y, Game *game, int health)
    : Entity(x, y, health), game(game) {
    lastShootClock = clock();
    shootInterval = CLOCKS_PER_SEC * RandomBetweenInt(1, 3);  // Intervallo reale in secondi
}

void Enemy::shoot() {
	Bullet bullet = Bullet(x, (y - 1), ENEMY_SHOOT_SPEED, 1, 1);
	if (game) {
		game->addBullet(bullet, ENEMY);
	}
}

void Enemy::update() {
	clock_t now = clock();
	if (now - lastShootClock >= shootInterval) {
		shoot();
		lastShootClock = now;
		shootInterval = CLOCKS_PER_SEC * RandomBetweenInt(1, 3);
	}
}