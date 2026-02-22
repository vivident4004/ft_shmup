#include "includes/enemy.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"
#include "includes/utilities.hpp"

Enemy::Enemy(int x, int y, Game *game, int health, bool isBoss)
    : Entity(x, y, health, isBoss), game(game) {
    shootInterval = RandomBetweenInt(1, 3);  // Intervallo in secondi
	if (isBoss) {
		shootInterval *= 3;
	}
    // Offset casuale per evitare spari sincronizzati
    double randomOffset = RandomBetweenInt(0, 100) * shootInterval / 100.0;
    lastShootTime = std::chrono::steady_clock::now() - std::chrono::milliseconds((int)(randomOffset * 1000));
}

void Enemy::shoot() {
	Bullet bullet = Bullet(x, (y - 1), RandomBetweenInt(5, 7), 1, 1);
	if (game) {
		game->addBullet(bullet, ENEMY);
	}
}

void Enemy::update() {
	auto now = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration<double>(now - lastShootTime).count();
	if (elapsed >= shootInterval) {
		shoot();
		lastShootTime = now;
		shootInterval = RandomBetweenInt(1, 10);
	}
}