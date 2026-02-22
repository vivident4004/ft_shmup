#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"

class Asteroid : public Entity
{
private:
    float speed;
    float y_pos;

public:
    Asteroid(int x, int y, int health, bool isBoss, float speed) : Entity(x, y, health, isBoss), speed(speed), y_pos((float)y) {}
    ~Asteroid() {}

    void shoot() override {}

    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }

    float getYPos() const { return y_pos; }
    void setYPos(float newY) { y_pos = newY; setY((int)newY); }
};

#endif
