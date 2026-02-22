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
	if (lives > 0)
	{
		// Ha ancora vite: respawn immediato
		lives--;
		health = maxHealth;
		x = WIN_WIDTH / 2;
		y = WIN_HEIGHT - 3;
	}
	else
	{
		// Niente vite: game over
		isDead = true;
		health = 0;
	}
}

void Player::revive()
{
	// Reset completo dopo game over
	isDead = false;
	health = maxHealth;
	x = WIN_WIDTH / 2;
	y = WIN_HEIGHT - 3;
	lives = 3;
}
