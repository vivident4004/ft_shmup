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
public:
    WINDOW *win;
    int curr_width;
    int curr_height;
    vector<Entity> entities;
    int score;

    Game() : score(0) {
        this->win = newwin(W_H, W_W, 0, 0);
        this->curr_width = W_W;
        this->curr_height = W_H;
    };
    ~Game() { delwin(this->win); };
    void updatePlayer(Player &player);
    void updateEnemies(int x, int y, vector<Bullet> &bullets);
    void updateBullets(vector<Bullet> &bullets);
    void drawPlayer(Player &player);
    void drawBullets(vector<Bullet> &bullets) ;
    void drawEnemies(vector<Entity> &enemies);
};

#endif