#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"
#include <chrono>

class Game; // Forward declaration

class Enemy : public Entity
{
private:
	Game *game;
	std::chrono::steady_clock::time_point lastShootTime;
	double shootInterval; // in secondi
	// bool isBoss;

public:
	Enemy(int x, int y, Game *game, int health);
	~Enemy() {}

	void shoot() override;
	void update();
};

#endif
