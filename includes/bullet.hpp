#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"

class Bullet : public Entity
{
public:
    int speed;
    int damage;
    int direction;
    int tick; // conta i fotogrammi per gestire la velocità

    Bullet(int x, int y, int speed, int damage, int direction)
        : Entity(x, y, 1), speed(speed), damage(damage), direction(direction), tick(0) {}

    void shoot() override {}
};

#endif
