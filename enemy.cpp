#include "includes/enemy.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"
#include "includes/utilities.hpp"

Enemy::Enemy(int x, int y, Game *game, int health, bool isBoss)
    : Entity(x, y, health, isBoss), game(game) {
    shootInterval = RandomBetweenInt(3, 5);  // Intervallo in secondi
	if (isBoss) {
		shootInterval = RandomBetweenInt(1, 2);  // Per boss, 1-3 secondi
	}
    // Offset casuale per evitare spari sincronizzati
    double randomOffset = RandomBetweenInt(0, 100) * shootInterval / 100.0;
    lastShootTime = std::chrono::steady_clock::now() - std::chrono::milliseconds((int)(randomOffset * 1000));
}

void Enemy::shoot() {
    if (isBoss) {
        // Boss spara tre raggi da posizioni diverse con velocità diverse
        for (int i = 0; i < 3; i++) {
            int speed = 5 + i; // Velocità 5, 6, 7
            int bulletX = x + (i - 1); // x-1, x, x+1
            Bullet bullet = Bullet(bulletX, y, speed, 1, 1);
            if (game) {
                game->addBullet(bullet, ENEMY);
            }
        }
    } else {
        // Nemico normale spara un proiettile
        Bullet bullet = Bullet(x, y, RandomBetweenInt(5, 7), 1, 1);
        if (game) {
            game->addBullet(bullet, ENEMY);
        }
    }
}

void Enemy::update() {
	auto now = std::chrono::steady_clock::now();
	double elapsed = std::chrono::duration<double>(now - lastShootTime).count();
	if (elapsed >= shootInterval) {
		shoot();
		lastShootTime = now;
		if (isBoss) {
			shootInterval = RandomBetweenInt(1, 2);
		} else {
			shootInterval = RandomBetweenInt(3, 5);
		}
	}
}