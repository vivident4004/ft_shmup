#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity
{
public:
    Player(int x, int y, int health = 100)  
    { 
        this->x = x; 
        this->y = y; 
        this->health = health;
        this->p_bullets = vector<Bullet>();
        this->e_bullets = vector<Bullet>();
    };
    ~Player(){ };
    
    void shoot() override {return ;};
     int getX() override {return 0;};
	 int getY()  override{return 0;}; 
	 int getHealth() override{return 0;}; 
	 void setX(int newX)override  {(void)newX; return ;}; 
	 void setY(int newY)override {(void) newY ;return ;}; 
	 void setHealth(int newHealth)override {(void) newHealth; return; }; 
	 vector<Bullet>& getPlayerBullets() override{return p_bullets; }; 
	 vector<Bullet>& getEnemyBullets()override {return e_bullets; }; 
};

#endif