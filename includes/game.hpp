#ifndef GAME_HPP
#define GAME_HPP

#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include "player.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include <ctime>

// Dimensioni della finestra di gioco
#define WIN_WIDTH 80
#define WIN_HEIGHT 24

#define PLAYER 0
#define PLAYER_SHOOT_SPEED 4
#define ENEMY 1
#define ENEMY_SHOOT_SPEED 4

class Game
{
public:
	WINDOW *win;
	int win_width;
	int win_height;
	bool paused;

	Player *player;
	std::vector<Bullet> p_bullets; // Proiettili del giocatore
	std::vector<Bullet> e_bullets; // Proiettili dei nemici
	std::vector<Enemy> enemies;

	int score;

	clock_t lastSpawnClock;
	clock_t spawnIntervalTicks;
	clock_t lastEnemyMoveClock;
	clock_t enemyMoveIntervalTicks;


	Game();
	~Game();

	// Gestione del ciclo di gioco
	void handleInput(int c);
	void update();
	void render();

	// Metodi di supporto
	void addBullet(const Bullet& bullet, int entity);
	void spawnEnemy();
	void updateBullets();
	void updateEnemies();
	void drawPlayer();
	void drawBullets();
	void drawEnemies();
	void checkCollisions();
};

#endif
