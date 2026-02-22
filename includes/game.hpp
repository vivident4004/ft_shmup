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
#define SPAWN_INTERVAL_FRAMES 200
#define ENEMY_MOVE_INTERVAL_FRAMES 60
#define MAX_ENEMIES 10

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
	bool bossWaveTriggered;
	unsigned int spawnedEnemies;

	bool isEasyMode;
	int menuSelection;
	int currentWave;
	int enemiesSpawnedInWave;
	int enemiesInWave;
	bool isBossWave;

	// Key states per input simultaneo
	bool keyLeft, keyRight, keyUp, keyDown, keyShooting;

	// Sfondo a stelle
	struct Star {
		int x, y;
		char ch;
		int color;
	};
	std::vector<Star> stars;
	unsigned long long bgFrameCount;

	// Frame-based timing
	unsigned long long frameCount;
	unsigned long long lastSpawnFrame;
	unsigned long long lastEnemyMoveFrame;

	// Metodi interni - update
	void updateEnemies();
	void updateBullets();
	void updateAsteroids(double deltaTime);
	void checkCollisions();

	// Metodi interni - draw
	void drawPlayer();
	void drawBullets();
	void drawEnemies();
	void drawBoss();
	void drawAsteroids();
	void drawHUD();
	void drawHealthBar();
	void drawLives();
	void drawPause();
	void drawExplosion();
	void drawGameOver();

	// Clock-based timing
	double currentFPS;
	double elapsedTime;
	double fpsAccumulator;
	int fpsFrameCount;

public:
	WINDOW *win;
	int win_width;
	int win_height;
	bool paused;
	int score;

	// Usati anche dal menu in main
	void drawBackground();
	void drawMenu();
	void updateBackground();
	bool initWindow();
	void renderMenu();

	void setDifficulty(int sel) { isEasyMode = (sel == 1); }
	int getMenuSelection() { return menuSelection; }
	void toggleMenuSelection() { menuSelection = 1 - menuSelection; }

	Game();
	~Game();

	void handleInput(int c);
	void update(double deltaTime);
	void render();

	void addBullet(const Bullet& bullet, int entity);
	void addAsteroid(const Asteroid& asteroid);
	void spawnEnemy();
	void spawnBoss();
};

#endif
