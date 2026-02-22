#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Game; // forward declaration

class Player : public Entity
{
private:
    Game *game;

public:
    Player(int x, int y, Game *game, int health);
    ~Player() {}
    
    // Metodo obbligatorio da implementare
    int getHealth() const override { return health; }
    void setHealth(int newHealth) override { health = newHealth; }
    void shoot() override;
    void loseLife(int damage);
    void die();
};

#endif
