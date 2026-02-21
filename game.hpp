#ifndef GAME_HPP
#define GAME_HPP

#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>

#include "entity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

#define W_W 40
#define W_H 20

using namespace std;

class Game
{
protected:
    WINDOW *win;
    int curr_width;
    int curr_height;
    vector<Entity> entities;
    int score;

    void updatePlayer(Player &player, vector<Bullet> &bullets);
    void updateEnemies(int x, int y, vector<Bullet> &bullets);
    void updateBullets(vector<Bullet> &bullets);
};

#endif