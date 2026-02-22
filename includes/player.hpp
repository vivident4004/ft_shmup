#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Game;

class Player : public Entity
{
private:
	Game *game;
	int maxHealth;
	bool isDead;
    int lives;

public:
	Player(int x, int y, Game *game, int health);
	~Player() {}

	void shoot() override;
	void loseLife(int damage);
	void die();
	void revive();

	bool getIsDead() const { return isDead; }
	int getMaxHealth() const { return maxHealth; }
};

#endif
