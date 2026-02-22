#ifndef GAME_HPP
#define GAME_HPP

#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include "player.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include "asteroid.hpp"
#include <ctime>
#include <chrono>

// Dimensioni della finestra di gioco
#define WIN_WIDTH 60
#define WIN_HEIGHT 50
#define FPS 60

// Frame-based timing (a 60 FPS: 60 frame = 1 secondo)
#define SPAWN_INTERVAL_FRAMES 120   // Spawn ogni 2 secondi
#define ENEMY_MOVE_INTERVAL_FRAMES 30 // Nemici si muovono ~20 volte/s

#define PLAYER 0
// #define PLAYER_SHOOT_SPEED 4
#define ENEMY 1
// #define ENEMY_SHOOT_SPEED 20

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
	std::vector<Asteroid> asteroids;

	// Stato dei tasti (permette input "simultanei")
	bool keyLeft;
	bool keyRight;
	bool keyUp;
	bool keyDown;
	bool keyShooting;

	int score;
	double currentFPS;
	double elapsedTime;     // Clock-based: tempo di gioco in secondi
	double fpsAccumulator;  // Clock-based: accumula delta per calcolo FPS
	int fpsFrameCount;      // Clock-based: frame contati nell'ultimo secondo

	unsigned long long frameCount;
	unsigned long long lastSpawnFrame;
	unsigned long long lastEnemyMoveFrame;

	Game();
	~Game();

	// Gestione del ciclo di gioco
	void handleInput(int c);
	void update();
	void render();

	// Metodi di supporto
	void addBullet(const Bullet& bullet, int entity);
	void addAsteroid(const Asteroid& asteroid);
	void spawnEnemy();
	void updateEnemies();
	void updateBullets();
	void updateAsteroids();
	void drawPlayer();
	void drawBullets();
	void drawEnemies();
	void drawAsteroids();
	void drawHUD();
	void checkCollisions();
	void drawHealthBar();
	void drawExplosion();
};

#endif
