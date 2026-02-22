#include "includes/player.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"

Player::Player(int x, int y, Game *game, int health) 
	: Entity(x, y, health, false), game(game), maxHealth(health), isDead(false), lives(3) {}

void Player::shoot()
{
	if (isDead) return;
	Bullet bullet = Bullet(x, (y - 1), 3, 1, -1);
	if (game) {
		game->addBullet(bullet, PLAYER);
	}
}

void Player::loseLife(int damage)
{
	health -= damage;
	if (health <= 0) {
		die();
	}
}

void Player::die()
{
	isDead = true;
	health = 0;
}

void Player::revive()
{
	if (lives > 0)
	{
		isDead = false;
		health = maxHealth;
		x = WIN_WIDTH / 2;   // Centro
		y = WIN_HEIGHT - 3;   // Basso
		lives--;
	}
	else
	{
		// Game over, reset completo
		isDead = false;
		health = maxHealth;
		x = WIN_WIDTH / 2;   // Centro
		y = WIN_HEIGHT - 3;   // Basso
		lives = 3; // Reset delle vite
	}
}
