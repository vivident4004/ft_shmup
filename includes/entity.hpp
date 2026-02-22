#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>

class Bullet; // Forward declaration if needed, though we moved vectors

class Entity
{
protected:
	int x;
	int y;
	int health;

public:
	Entity(int x = 0, int y = 0, int health = 100) : x(x), y(y), health(health) {}
	virtual ~Entity() {}

	// Metodi puramente virtuali: rende la classe astratta.
	// Ogni sottoclasse dovrà decidere come sparare.
	virtual void shoot() = 0;

	virtual int getX() const { return x; }
	virtual int getY() const { return y; }
	virtual int getHealth() const { return health; }

	virtual void setX(int newX) { x = newX; }
	virtual void setY(int newY) { y = newY; }
	virtual void setHealth(int newHealth) { health = newHealth; }
};

#endif
