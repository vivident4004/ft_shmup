#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"

class Enemy : public Entity
{
public:
    Enemy(int x, int y, int health = 10) : Entity(x, y, health) {}
    ~Enemy() {}

    void shoot() override {
        // Logica per sparare (da implementare dall'utente)
    }
};

#endif
