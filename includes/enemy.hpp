#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"
#include <ctime>

class Game; // Forward declaration

class Enemy : public Entity
{
private:
	Game *game;
	clock_t lastShootClock;
	int shootInterval;
public:
	Enemy(int x, int y, Game *game, int health = 10);
	~Enemy() {}

	void shoot() override;
	void update();
};

#endif
