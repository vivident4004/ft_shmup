#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"

class Bullet : public Entity
{
public:
	int speed;
	int damage;
	int direction;
	Bullet(int x, int y, int speed, int damage, int direction) : Entity(), speed(speed), damage(damage), direction(direction) {};
};

#endif