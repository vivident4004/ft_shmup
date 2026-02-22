#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"

class Asteroid : public Entity
{
public:
    Asteroid(int x, int y, int health, bool isBoss) : Entity(x, y, health, isBoss) {}
    ~Asteroid() {}

    void shoot() override {}
};

#endif
