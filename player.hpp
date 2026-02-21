#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity
{
public:
    Player(int x, int y, int health = 100) : Entity() 
    { 
        Entity::x = x; 
        Entity::y = y; 
        Entity::health = health;
    };
    
    void shoot() override;
};

#endif