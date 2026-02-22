#include "includes/game.hpp"
#include "includes/utilities.hpp"

Game::Game() : player(nullptr),
				bossWaveTriggered(false), spawnedEnemies(0),
				isEasyMode(false), menuSelection(0), currentWave(1),
				enemiesSpawnedInWave(0), enemiesInWave(MAX_ENEMIES), isBossWave(false),
				keyLeft(false), keyRight(false), keyUp(false),
				keyDown(false), keyShooting(false),
				bgFrameCount(0),
				frameCount(0), lastSpawnFrame(0), lastEnemyMoveFrame(0),
				currentFPS(0.0), elapsedTime(0.0),
				fpsAccumulator(0.0), fpsFrameCount(0),
				win(nullptr), win_width(WIN_WIDTH), win_height(WIN_HEIGHT),
				paused(false), score(0)
{
	this->player = new Player(WIN_WIDTH / 2, WIN_HEIGHT - 3, this, 100);

	// Inizializza stelle di sfondo
	const char starChars[] = {'.', '+', '*', '`'};
	for (int i = 0; i < MAX_STARS; i++)
	{
		Star s;
		s.x = RandomBetweenInt(1, WIN_WIDTH - 2);
		s.y = RandomBetweenInt(1, WIN_HEIGHT - 2);
		s.ch = starChars[RandomBetweenInt(0, 3)];
		s.color = (RandomBetweenInt(0, 2) == 0) ? COLOR_STAR_BRIGHT : COLOR_STAR_DIM;
		stars.push_back(s);
	}
}

Game::~Game()
{
	if (this->win)
	{
		delwin(this->win);
	}
	delete this->player;
}

void Game::addBullet(const Bullet &bullet, int entity)
{
	if (entity == PLAYER)
		p_bullets.push_back(bullet);
	else if (entity == ENEMY)
		e_bullets.push_back(bullet);
}

void Game::addAsteroid(const Asteroid &asteroid)
{
	asteroids.push_back(asteroid);
}

void Game::spawnEnemy()
{
	int x = RandomBetweenInt(1, win_width - 2);
	enemies.emplace_back(x, 1, this, 5, false);
	spawnedEnemies++;
}

void Game::spawnBoss()
{
	int x = RandomBetweenInt(1, win_width - 2);
	enemies.emplace_back(x, 2, this, 20, true); // Boss con più salute
}

void Game::handleInput(int c)
{
	if (player->getIsDead())
	{
		if (c == 'r' || c == 'R')
		{
			player->revive();
			score = 0;
			frameCount = 0;
			lastSpawnFrame = 0;
			lastEnemyMoveFrame = 0;
			elapsedTime = 0.0;
			enemies.clear();
			p_bullets.clear();
			e_bullets.clear();
			asteroids.clear();
			bossWaveTriggered = false;
			spawnedEnemies = 0;
			currentWave = 1;
			enemiesSpawnedInWave = 0;
			enemiesInWave = 3;
			isBossWave = false;
		}
		return;
	}

	// Input normale del gioco
	switch (c)
	{
	case KEY_LEFT:
		if (player->getX() > 2)
			player->setX(player->getX() - 1);
		break;
	case KEY_RIGHT:
		if (player->getX() < win_width - 3)
			player->setX(player->getX() + 1);
		break;
	case KEY_UP:
		if (player->getY() > 1)
			player->setY(player->getY() - 1);
		break;
	case KEY_DOWN:
		if (player->getY() < win_height - 3)
			player->setY(player->getY() + 1);
		break;
	case ' ':
		player->shoot();
		break;
	case 'p':
		paused = !paused;
		break;
	}
}

void Game::update(double deltaTime)
{
	frameCount++;

	// Clock-based: accumula tempo di gioco e calcola FPS
	elapsedTime += deltaTime;
	fpsAccumulator += deltaTime;
	fpsFrameCount++;
	if (fpsAccumulator >= 1.0) {
		currentFPS = fpsFrameCount / fpsAccumulator;
		fpsFrameCount = 0;
		fpsAccumulator = 0.0;
	}

	// Aggiorna sfondo stellare
	updateBackground();

	if (frameCount - lastSpawnFrame >= (unsigned long long)RandomBetweenInt(60, SPAWN_INTERVAL_FRAMES))
	{

		if (enemiesSpawnedInWave < enemiesInWave)
		{
			spawnEnemy();
			enemiesSpawnedInWave++;
		}
		else if (this->enemies.size() == 0)
		{
			if (isBossWave)
			{
				// next wave
				currentWave++;
				enemiesInWave += 2;
				enemiesSpawnedInWave = 0;
				isBossWave = false;
				bossWaveTriggered = false;
			}
			else
			{
				if (!isEasyMode)
				{
					spawnBoss();
					isBossWave = true;
					bossWaveTriggered = true;
				}
				else
				{
					// next wave without boss
					currentWave++;
					enemiesInWave += 2;
					enemiesSpawnedInWave = 0;
					bossWaveTriggered = false;
				}
			}
		}
		addAsteroid(Asteroid(RandomBetweenInt(1, win_width - 2), 1, 5, false, 10.0f + (float)RandomBetweenInt(0, 20)));
		lastSpawnFrame = frameCount;
	}

	// updateBullet() è chiamato ogni frame
	updateBullets();
	updateAsteroids(deltaTime);
	checkCollisions();

	if (frameCount - lastEnemyMoveFrame >= ENEMY_MOVE_INTERVAL_FRAMES)
	{
		updateEnemies();
		lastEnemyMoveFrame = frameCount;
	}
}
