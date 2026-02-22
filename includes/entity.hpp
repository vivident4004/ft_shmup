#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
protected:
	int x;
	int y;
	int health;

public:
	Entity(int x = 0, int y = 0, int health = 100) : x(x), y(y), health(health) {}
	virtual ~Entity() {}

	virtual void shoot() = 0;

	int getX() const { return x; }
	int getY() const { return y; }
	int getHealth() const { return health; }

	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setHealth(int newHealth) { health = newHealth; }
};

#endif