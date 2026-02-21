#ifndef BULLET_HPP
#define BULLET_HPP

#include "entity.hpp"

class Bullet : public Entity
{
public:
	int speed;
	int damage;
	int direction;
	Bullet(int x, int y, int speed, int damage, int direction) :  speed(speed), damage(damage), direction(direction) {
		this->x = x;
		this->y = y;
		this->health = 1; 
	};
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