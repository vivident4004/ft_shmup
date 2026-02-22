#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Game; // forward declaration

class Player : public Entity
{
private:
    Game *game;

public:
    Player(int x, int y, Game *game, int health) : Entity(x, y, health), game(game) {}
    ~Player() {}
    
    // Metodo obbligatorio da implementare
    void shoot() override;
    void loseLife(int damage = 10);
    void die();
};

#endif
