#ifndef GAME_HPP
#define GAME_HPP

#include <ncurses.h>
#include <vector>
#include <chrono>
#include "player.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include "asteroid.hpp"

// Dimensioni della finestra di gioco
#define WIN_WIDTH 60
#define WIN_HEIGHT 50
#define FPS 60

// Frame-based timing (a 60 FPS: 60 frame = 1 secondo)
#define SPAWN_INTERVAL_FRAMES 120
#define ENEMY_MOVE_INTERVAL_FRAMES 30

#define PLAYER 0
#define ENEMY 1

// Color pairs
#define COLOR_PLAYER 1
#define COLOR_PLAYER_BULLET 2
#define COLOR_ENEMY_FULL 3
#define COLOR_ENEMY_BULLET 4
#define COLOR_ASTEROID 5
#define COLOR_HUD 6
#define COLOR_HP_FULL 7
#define COLOR_HP_LOW 8
#define COLOR_STAR_DIM 9
#define COLOR_STAR_BRIGHT 10
#define COLOR_EXPLOSION 11
#define COLOR_GAMEOVER 12
#define COLOR_ENEMY_DAMAGED 13
#define COLOR_ENEMY_DEAD 14
#define COLOR_BROWN 15

#define MAX_STARS 40

class Game
{
private:
	Player *player;
	std::vector<Bullet> p_bullets;
	std::vector<Bullet> e_bullets;
	std::vector<Enemy> enemies;
	std::vector<Asteroid> asteroids;

	// Sfondo a stelle
	struct Star {
		int x, y;
		char ch;
		int color;
	};
	std::vector<Star> stars;

	// Stato dei tasti
	bool keyLeft;
	bool keyRight;
	bool keyUp;
	bool keyDown;
	bool keyShooting;

	// Frame-based timing
	unsigned long long frameCount;
	unsigned long long lastSpawnFrame;
	unsigned long long lastEnemyMoveFrame;

	// Metodi interni - update
	void updateEnemies();
	void updateBullets();
	void updateAsteroids();
	void checkCollisions();

	// Metodi interni - draw
	void drawPlayer();
	void drawBullets();
	void drawEnemies();
	void drawAsteroids();
	void drawHUD();
	void drawHealthBar();
	void drawLives();
	void drawPause();
	void drawExplosion();
	void drawBackground();
	void drawGameOver();
	void updateBackground();
	bool initWindow();

public:
	WINDOW *win;
	int win_width;
	int win_height;
	bool paused;
	int score;

	// Clock-based timing
	double currentFPS;
	double elapsedTime;
	double fpsAccumulator;
	int fpsFrameCount;

	Game();
	~Game();

	void handleInput(int c);
	void update();
	void render();

	void addBullet(const Bullet& bullet, int entity);
	void addAsteroid(const Asteroid& asteroid);
	void spawnEnemy();
	void spawnBoss();
};

#endif
