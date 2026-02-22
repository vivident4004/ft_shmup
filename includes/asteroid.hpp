#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entity.hpp"

class Asteroid : public Entity
{
public:
    Asteroid(int x, int y, int health = 5) : Entity(x, y, health) {}
    ~Asteroid() {}

    void shoot() override {
        // Gli asteroidi di solito non sparano
    }
};

#endif
