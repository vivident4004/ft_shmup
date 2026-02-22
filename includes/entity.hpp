#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
protected:
	int x;
	int y;
	int health;
	bool isBoss;

public:
	Entity(int x, int y, int health, bool isBoss) : x(x), y(y), health(health), isBoss(isBoss) {}
	virtual ~Entity() {}

	virtual void shoot() = 0;

	int getX() const { return x; }
	int getY() const { return y; }
	int getHealth() const { return health; }
	bool getIsBoss() const { return isBoss; }

	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setHealth(int newHealth) { health = newHealth; }
};

#endif