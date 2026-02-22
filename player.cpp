#include "includes/player.hpp"
#include "includes/bullet.hpp"
#include "includes/game.hpp"

Player::Player(int x, int y, Game *game, int health) : Entity(x, y, health), game(game) {}

void Player::shoot()
{
	// Genera un proiettile e lo aggiunge al gioco tramite il puntatore a Game.
	Bullet bullet = Bullet(x, (y - 1), PLAYER_SHOOT_SPEED, 1, -1);
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
	// Per ora, semplicemente resetta la salute e la posizione del giocatore.
	printf("Game Over! You died.\n");
	health = 2;
	x = game->win_width / 2;
	y = game->win_height - 3;
	// Potresti anche voler resettare il punteggio o altri stati di gioco qui.
}
