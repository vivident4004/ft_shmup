#include "includes/player.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"

Player::Player(int x, int y, Game *game, int health) 
	: Entity(x, y, health), game(game), maxHealth(health), isDead(false) {}

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
	isDead = false;
	health = maxHealth;
	x = 40;   // Centro
	y = 20;   // Basso
}
