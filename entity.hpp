// INTERFACE
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>

class Bullet; // Forward declaration

using namespace std;

class Entity
{
protected:
	int x;
	int y;
	int health;
	vector<Bullet> e_bullets;
	vector<Bullet> p_bullets;

public:
// virtual void move(int dx, int dy);
virtual ~Entity() {};

virtual void shoot() = 0;
	virtual int getX() = 0;
	virtual int getY()  = 0; 
	virtual int getHealth() = 0; 
	virtual void setX(int newX) = 0; 
	virtual void setY(int newY) = 0; 
	virtual void setHealth(int newHealth) = 0; 
	virtual vector<Bullet>& getPlayerBullets() = 0; 
	virtual vector<Bullet>& getEnemyBullets() = 0; 
};

#endif