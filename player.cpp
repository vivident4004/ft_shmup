#include "includes/player.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"

void Player::shoot()
{
	// Genera un proiettile e lo aggiunge al gioco tramite il puntatore a Game.
	Bullet bullet = Bullet(x, (y - 1), 3, 1, -1);
	if (game) {
		game->addBullet(bullet);
	}
}
