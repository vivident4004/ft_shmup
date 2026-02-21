// INTERFACE
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "bullet.hpp"

class Bullet; // Forward declaration

class Entity
{
protected:
	int x;
	int y;
	int health;
	vector<Bullet> e_bullets;
	vector<Bullet> p_bullets;

public:
	Entity() : x(0), y(0), health(100) {};
	// Entity(int x, int y, int health) : x(x), y(y), health(health) {};
	virtual void shoot(int direction);
	virtual void move(int dx, int dy);

	int getX() const { return x; }
	int getY() const { return y; }
	int getHealth() const { return health; }
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setHealth(int newHealth) { health = newHealth; }
};

#endif